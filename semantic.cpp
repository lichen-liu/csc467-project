#include "ast.h"

#include "symbol.h"

namespace SA{ /* START NAMESPACE */

class SymbolDeclVisitor: public AST::Visitor {
    private:
        ST::SymbolTable &m_symbolTable;
    public:
        SymbolDeclVisitor(ST::SymbolTable &symbolTable):
            m_symbolTable(symbolTable) {}

    public:
        virtual void visit(AST::IdentifierNode *identifierNode) {}
        virtual void visit(AST::DeclarationNode *declarationNode) {}
        virtual void visit(AST::NestedScopeNode *nestedScopeNode) {}
        virtual void visit(AST::ScopeNode *scopeNode) {}
};

} /* END NAMESPACE */

int semantic_check(node * ast) {
    ST::SymbolTable symbolTable;

    SA::SymbolDeclVisitor symbolDeclVisitor(symbolTable);

    static_cast<AST::ASTNode *>(ast)->visit(symbolDeclVisitor);

    return 1;
}