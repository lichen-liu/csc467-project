#include "semantic.h"

#include "ast.h"
#include "symbol.h"

namespace SA{ /* START NAMESPACE */

class SymbolDeclVisitor: public AST::Visitor {
    private:
        ST::SymbolTable &m_symbolTable;
    public:
        SymbolDeclVisitor(ST::SymbolTable &symbolTable):
            m_symbolTable(symbolTable) {}

    private:
        virtual void preNodeVisit(AST::IdentifierNode *identifierNode) {
            m_symbolTable.markSymbolRefPos(identifierNode);
        }

        virtual void postNodeVisit(AST::DeclarationNode *declarationNode) {
            // Traverse the possible evaluation on rhs before the declaration of symbol
            m_symbolTable.declareSymbol(declarationNode);
        }

        virtual void preNodeVisit(AST::NestedScopeNode *nestedScopeNode) {
            m_symbolTable.enterScope();
        }
        virtual void postNodeVisit(AST::NestedScopeNode *nestedScopeNode) {
            m_symbolTable.exitScope();
        }

        virtual void preNodeVisit(AST::ScopeNode *scopeNode) {
            m_symbolTable.enterScope();
        }
        virtual void postNodeVisit(AST::ScopeNode *scopeNode) {
            m_symbolTable.exitScope();
        }
};

class SymbolLUVisitor: public AST::Visitor {
    private:
        ST::SymbolTable &m_symbolTable;
    public:
        SymbolLUVisitor(ST::SymbolTable &symbolTable):
            m_symbolTable(symbolTable) {}
    
    private:
        virtual void preNodeVisit(AST::IdentifierNode *identifierNode) {
            m_symbolTable.getSymbolDecl(identifierNode);
        }
};

} /* END NAMESPACE */

int semantic_check(node * ast) {
    ST::SymbolTable symbolTable;

    /* Construct Symbol Tree */
    SA::SymbolDeclVisitor symbolDeclVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolDeclVisitor);
    // symbolTable.printScopeLeaves();

    /* Symbol Type Look Up */
    SA::SymbolLUVisitor symbolLUVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolLUVisitor);
    symbolTable.printSymbolReference();

    return 1;
}