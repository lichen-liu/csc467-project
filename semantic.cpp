#include "semantic.h"

#include "ast.h"
#include "symbol.h"

#include <cassert>

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
            bool successful = m_symbolTable.declareSymbol(declarationNode);

            /// TODO: print error
            if(!successful) {
                printf("Error: Redeclaration for DeclarationNode(%p):\n");
                ast_print(declarationNode);
                printf("\n");

                return;
            }
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
            AST::DeclarationNode *decl = m_symbolTable.getSymbolDecl(identifierNode);

            /// TODO: print error
            if(decl == nullptr) {
                printf("Error: Missing Declaration for IdentifierNode(%p)\n", identifierNode);
                ast_print(identifierNode);
                printf("\n");

                return;
            }

            assert(decl->getName() == identifierNode->getName());

            // Update info in identifierNode
            identifierNode->setExpressionType(decl->getType());
            identifierNode->setConst(decl->isConst());
            identifierNode->setDeclaration(decl);
        }
};

} /* END NAMESPACE */

int semantic_check(node * ast) {
    /*
     * TODO
     * 
     * Better structure to wrap around errors with context info.
     * E.g. Line, Col, Meaningful Error Messages.
     * 
     */

    ST::SymbolTable symbolTable;

    /* Construct Symbol Tree */
    SA::SymbolDeclVisitor symbolDeclVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolDeclVisitor);
    symbolTable.printScopeLeaves();

    /* Symbol Look Up */
    SA::SymbolLUVisitor symbolLUVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolLUVisitor);
    symbolTable.printSymbolReference();

    /* Type Inference */
    // should combine with symbol look-up?

    return 1;
}