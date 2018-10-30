#include "symbol.h"
#include "ast.h"

#include <cassert>
#include <vector>
#include <forward_list>
#include <unordered_map>
#include <memory>

namespace ST{ /* START NAMESPACE */

class SymbolNode {
    private:
        SymbolNode *m_prevSymbolNode = nullptr;                     // Link to the previous SymbolNode in SymbolTable
        AST::DeclarationNode *m_decl = nullptr;                     // Reference to the DeclarationNode in AST

    public:
        SymbolNode() = default;
        SymbolNode(SymbolNode *prevSymbolNode, AST::DeclarationNode *decl):
            m_prevSymbolNode(prevSymbolNode), m_decl(decl) {}
    
    public:
        SymbolNode *getPrevSymbolNode() const { return m_prevSymbolNode; }
        AST::DeclarationNode *getDecl() const { return m_decl; }

};

class SymbolTable {
    private:
        std::vector<std::unique_ptr<SymbolNode>> m_symbolNodes;     // Ownership of all SymbolNodes
    private:
        std::forward_list<SymbolNode *> m_scope;                    // Scopes
        SymbolNode *m_currentHead = nullptr;                        // Current SymbolNode
    private:
        std::unordered_map<AST::IdentifierNode *, SymbolNode *> m_positionOfRef;
                                                                    // Position of reference of Identifier
    
    public:
        void clear();

    public:
        /* Declaration Related */
        void enterScope();
        void exitScope();
        bool declareSymbol(AST::DeclarationNode *decl);
    
    public:
        /* Identifier Related */
        void markSymbolRefPos(AST::IdentifierNode *ident);
        AST::DeclarationNode *getSymbolDecl(AST::IdentifierNode *ident) const;

    private:
        /* Helper Functions */
        /* Return redecl if redeclaration under current scope; otherwise nullptr */
        AST::DeclarationNode *findRedeclaration(AST::DeclarationNode *decl) const;
        static bool checkSymbolMatch(AST::DeclarationNode *decl, AST::IdentifierNode *ident);
};

void SymbolTable::clear() {
    m_symbolNodes.clear();
    m_scope.clear();
    m_currentHead = nullptr;
    m_positionOfRef.clear();
}

void SymbolTable::enterScope() {
    /* Push the last symbol (currentHead) of a scope onto the stack */
    m_scope.push_front(m_currentHead);
}

void SymbolTable::exitScope() {
    /* Pop the last symbol of a scope from the stack, and make it the currentHead */
    m_currentHead = m_scope.front();
    m_scope.pop_front();
}

bool SymbolTable::declareSymbol(AST::DeclarationNode *decl) {
    assert(decl != nullptr);

    AST::DeclarationNode *redecl = findRedeclaration(decl);
    if(redecl != nullptr) {
        return false;
    }

    std::unique_ptr<SymbolNode> uptr(new SymbolNode(m_currentHead, decl));
    m_currentHead = uptr.get();
    m_symbolNodes.push_back(std::move(uptr));

    return true;
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