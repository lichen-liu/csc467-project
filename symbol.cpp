#include "symbol.h"

#include <cassert>

namespace ST{ /* START NAMESPACE */

class SymbolNode {
    private:
        SymbolNode *m_prevSymbolNode = nullptr;                     // Link to the previous SymbolNode in SymbolTable
        AST::DeclarationNode *m_decl = nullptr;                     // Reference to the DeclarationNode in AST
        bool m_isFirstInScope        = false;                       // Whether this node is the first symbol of a new scope

    public:
        SymbolNode() = default;
        SymbolNode(SymbolNode *prevSymbolNode, AST::DeclarationNode *decl, bool isFirstInScope):
            m_prevSymbolNode(prevSymbolNode), m_decl(decl), m_isFirstInScope(isFirstInScope) {}
    
    public:
        SymbolNode *getPrevSymbolNode() const { return m_prevSymbolNode; }
        AST::DeclarationNode *getDecl() const { return m_decl; }
        bool isFirstInScope() const { return m_isFirstInScope; }
};

SymbolTable::SymbolTable() = default;
SymbolTable::~SymbolTable() = default;

void SymbolTable::clear() {
    m_symbolNodes.clear();
    m_scope.clear();
    m_symbolTreeScopeLeaves.clear();
    m_encounterNewScope = false;
    m_currentHead = nullptr;
    m_positionOfRef.clear();
}

void SymbolTable::enterScope() {
    /* Push the last symbol (currentHead) of a scope onto the stack */
    m_scope.push_front(m_currentHead);
    m_encounterNewScope = true;
}

void SymbolTable::exitScope() {
    /* Pop the last symbol of a scope from the stack, and make it the currentHead */
    m_symbolTreeScopeLeaves.push_back(m_currentHead);
    m_currentHead = m_scope.front();
    m_scope.pop_front();
    m_encounterNewScope = false;
}

AST::DeclarationNode *SymbolTable::declareSymbol(AST::DeclarationNode *decl) {
    assert(decl != nullptr);

    AST::DeclarationNode *redecl = findRedeclaration(decl);
    if(redecl != nullptr) {
        return redecl;
    }

    std::unique_ptr<SymbolNode> uptr(new SymbolNode(m_currentHead, decl, m_encounterNewScope));
    m_encounterNewScope = false;
    m_currentHead = uptr.get();
    m_symbolNodes.push_back(std::move(uptr));

    return nullptr;
}

void SymbolTable::markSymbolRefPos(AST::IdentifierNode *ident) {
    assert(ident != nullptr);

    assert(m_positionOfRef.count(ident) == 0);
    m_positionOfRef.emplace(ident, m_currentHead);
}

AST::DeclarationNode *SymbolTable::getSymbolDecl(AST::IdentifierNode *ident) const {
    assert(ident != nullptr);

    assert(m_positionOfRef.count(ident) == 1);

    SymbolNode *symNode = m_positionOfRef.at(ident);
    AST::DeclarationNode *resultDecl = nullptr;

    while(symNode != nullptr) {
        AST::DeclarationNode *decl = symNode->getDecl();
        if(checkSymbolMatch(decl, ident)) {
            resultDecl = decl;
            break;
        }

        symNode = symNode->getPrevSymbolNode();
    }

    return resultDecl;
}

int SymbolTable::printSymbolTreeTo(SymbolNode *node, const AST::DeclarationNode *markDecl) {
    if(node == nullptr) {
        return 0;
    }
    int scopeCount = printSymbolTreeTo(node->getPrevSymbolNode(), markDecl);
    if(node->isFirstInScope()) {
        scopeCount++;
        printf("%s", std::string(scopeCount * 3 - 2, ' ').c_str());
        printf("`--<%d>------------------------------------\n", scopeCount - 1);
        printf("%s", std::string(scopeCount * 3 - 1, ' ').c_str());
        printf("`--");
    } else {
        printf("%s", std::string(scopeCount * 3, ' ').c_str());
        printf("|-");
    }

    ast_print(node->getDecl());
    if(node->getDecl() == markDecl) {
        printf("        <-");
    }
    printf("\n");
    return scopeCount;
}

void SymbolTable::printScopeLeaves() const {
    printf("### Symbol Table Scope Leaves: %d Leaves ###\n", m_symbolTreeScopeLeaves.size());
    int i = 0;
    for(SymbolNode *node: m_symbolTreeScopeLeaves) {
        printf("========================================\n");
        printf("Root\n");
        int scopeCount = printSymbolTreeTo(node);
        printf("%s", std::string(scopeCount * 3, ' ').c_str());
        printf("   `==>");
        printf("Leaf[%d]\n", i);

        i++;
    }
}

void SymbolTable::printSymbolReference() const {
    printf("### Symbol Table Symbol Reference: %d References ###\n", m_positionOfRef.size());
    int i = 0;
    for(const auto &idSymPair: m_positionOfRef) {
        printf("========================================\n");
        printf("Root\n");
        AST::IdentifierNode *identNode = idSymPair.first;
        int scopeCount = printSymbolTreeTo(idSymPair.second, identNode->getDeclaration());
        printf("%s", std::string(scopeCount * 3, ' ').c_str());
        printf("   `==>");
        printf("Ref[%d]: ", i);
        printf("<%s", identNode->isConst() ? "const " : "");
        printf("%s> ", AST::getTypeString(identNode->getExpressionType()).c_str());
        printf("%s\n", identNode->getName().c_str());

        i++;
    }
}

AST::DeclarationNode *SymbolTable::findRedeclaration(AST::DeclarationNode *decl) const {
    SymbolNode *scopeEnd = m_scope.front();
    SymbolNode *symNode = m_currentHead;

    AST::DeclarationNode *redecl = nullptr;

    while(symNode != scopeEnd) {
        AST::DeclarationNode *curDecl = symNode->getDecl();

        if(curDecl->getName() == decl->getName()) {
            redecl = curDecl;
            break;
        }

        symNode = symNode->getPrevSymbolNode();
    }

    return redecl;
}

bool SymbolTable::checkSymbolMatch(AST::DeclarationNode *decl, AST::IdentifierNode *ident) {
    return (ident->getName() == decl->getName());
}

} /* END NAMESPACE */