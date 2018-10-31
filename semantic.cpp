#include "semantic.h"

#include "ast.h"
#include "symbol.h"

#include "common.h"
#include "parser.tab.h"

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

enum class DataTypeCategory {
    logical,
    arithmetic
};

DataTypeCategory getDataTypeCategory(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case BVEC2_T:
        case BVEC3_T:
        case BVEC4_T:
            return DataTypeCategory::logical;
        case INT_T:
        case IVEC2_T:
        case IVEC3_T:
        case IVEC4_T:
        case FLOAT_T:
        case VEC2_T:
        case VEC3_T:
        case VEC4_T:
            return DataTypeCategory::arithmetic;
        default:
            assert(0);
    }
}

int getDataTypeOrder(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case INT_T:
        case FLOAT_T:
            return 1;
        case BVEC2_T:
        case IVEC2_T:
        case VEC2_T:
            return 2;
        case BVEC3_T:
        case IVEC3_T:
        case VEC3_T:
            return 3;
        case BVEC4_T:
        case IVEC4_T:
        case VEC4_T:
            return 4;
        default:
            assert(0);
    }
}

int getDataTypeBaseType(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case BVEC2_T:
        case BVEC3_T:
        case BVEC4_T:
            return BOOL_T;
        case INT_T:
        case IVEC2_T:
        case IVEC3_T:
        case IVEC4_T:
            return INT_T;
        case FLOAT_T:
        case VEC2_T:
        case VEC3_T:
        case VEC4_T:
            return FLOAT_T;
        default:
            assert(0);
    }
}

int inferDataType(int op, int rhsDataType) {
    /* Unary Operator Type Inference */
    /*
     * - s, v Arithmetic
     * ! s, v Logical
     */

    if(rhsDataType == ANY_TYPE) {
        return ANY_TYPE;
    }

    DataTypeCategory typeCateg = getDataTypeCategory(rhsDataType);
    switch(op) {
        case MINUS: {
            if(typeCateg == DataTypeCategory::arithmetic) {
                return rhsDataType;
            }
            
            break;
        }
        case NOT: {
            if(typeCateg == DataTypeCategory::logical) {
                return rhsDataType;
            }

            break;
        }
        default:
            assert(0);
    }

    return ANY_TYPE;
}

class TypeInferenceVisitor: public AST::Visitor {    
    private:
        virtual void postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode){
            const AST::ExpressionNode *rhsExpr = unaryExpressionNode->getExpression();
            int rhsDataType = rhsExpr->getExpressionType();
            bool rhsIsConst = rhsExpr->isConst();
            int op = unaryExpressionNode->getOperator();

            int resultDataType = inferDataType(op, rhsDataType);
            unaryExpressionNode->setExpressionType(resultDataType);
            unaryExpressionNode->setConst(rhsIsConst);
        }

        virtual void postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode){}
        virtual void postNodeVisit(AST::IntLiteralNode *intLiteralNode){}
        virtual void postNodeVisit(AST::FloatLiteralNode *floatLiteralNode){}
        virtual void postNodeVisit(AST::BooleanLiteralNode *booleanLiteralNode){}
        virtual void postNodeVisit(AST::IdentifierNode *identifierNode){}
        virtual void postNodeVisit(AST::IndexingNode *indexingNode){}
        virtual void postNodeVisit(AST::FunctionNode *functionNode){}
        virtual void postNodeVisit(AST::ConstructorNode *constructorNode){}
        virtual void postNodeVisit(AST::DeclarationNode *declarationNode){}
        virtual void postNodeVisit(AST::IfStatementNode *ifStatementNode){}
        virtual void postNodeVisit(AST::AssignmentNode *assignmentNode){}
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
    // symbolTable.printScopeLeaves();

    /* Symbol Look Up */
    SA::SymbolLUVisitor symbolLUVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolLUVisitor);
    // symbolTable.printSymbolReference();

    /* Type Inference */
    SA::TypeInferenceVisitor typeInferenceVisitor;
    static_cast<AST::ASTNode *>(ast)->visit(typeInferenceVisitor);
    ast_print(ast);

    return 1;
}