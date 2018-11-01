#include "semantic.h"

#include "ast.h"
#include "symbol.h"

#include "common.h"
#include "parser.tab.h"

#include <cassert>

namespace SEMA{ /* START NAMESPACE */

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
    boolean,
    arithmetic
};

DataTypeCategory getDataTypeCategory(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case BVEC2_T:
        case BVEC3_T:
        case BVEC4_T:
            return DataTypeCategory::boolean;
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
            if(typeCateg == DataTypeCategory::boolean) {
                return rhsDataType;
            }

            break;
        }
        default:
            assert(0);
    }

    return ANY_TYPE;
}

int inferDataType(int op, int lhsDataType, int rhsDataType) {
    /* Binary Operator Type Inference */
    /*
     * +, - ss, vv Arithmetic
     * * ss, vv, sv, vs Arithmetic
     * /, ˆ ss Arithmetic
     * &&, || ss, vv Logical
     * <, <=, >, >= ss Comparison
     * ==, != ss, vv Comparison
     */

    if(lhsDataType == ANY_TYPE || rhsDataType == ANY_TYPE) {
        return ANY_TYPE;
    }

    if(getDataTypeBaseType(lhsDataType) != getDataTypeBaseType(rhsDataType)) {
        /*
         * Both operands of a binary operator must have exactly the same base type (e.g. it
         * is valid to multiple an int and an ivec3)
         */
        return ANY_TYPE;
    }

    int lhsTypeOrder = getDataTypeOrder(lhsDataType);
    int rhsTypeOrder = getDataTypeOrder(rhsDataType);
    if(lhsTypeOrder > 1 && rhsTypeOrder > 1) {
       if(lhsTypeOrder != rhsTypeOrder) {
            /*
            * If both arguments to a binary operator are vectors, then they must be vectors of the
            * same order. For example, it is not valid to add an ivec2 with an ivec3
            */
           return ANY_TYPE;
       }
    }

    DataTypeCategory lhsTypeCateg = getDataTypeCategory(lhsDataType);
    DataTypeCategory rhsTypeCateg = getDataTypeCategory(rhsDataType);
    switch(op) {
        case PLUS:
        case MINUS: {
            if(lhsTypeCateg == DataTypeCategory::arithmetic && rhsTypeCateg == DataTypeCategory::arithmetic) {
                if(lhsTypeOrder == rhsTypeOrder) {
                    // Same base type
                    // Both arithmetic type
                    // Same type order
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }
            break;
        }

        case TIMES: {
            if(lhsTypeCateg == DataTypeCategory::arithmetic && rhsTypeCateg == DataTypeCategory::arithmetic) {
                if(lhsTypeOrder > rhsTypeOrder) {
                    // Same base type
                    // Both arithmetic type
                    // If both vector, same type order; otherwise, any order combination eg. ss and sv
                    // Return the larger order one
                    return lhsDataType;
                } else {
                    return rhsDataType;
                }
            }
            break;
        }

        case SLASH:
        case EXP: {
            if(lhsTypeCateg == DataTypeCategory::arithmetic && rhsTypeCateg == DataTypeCategory::arithmetic) {
                if(lhsTypeOrder == 1 && rhsTypeOrder == 1) {
                    // Same base type
                    // Both arithmetic type
                    // Both scalar type
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }
            break;
        }

        case AND:
        case OR: {
            if(lhsTypeCateg == DataTypeCategory::boolean && rhsTypeCateg == DataTypeCategory::boolean) {
                if(lhsTypeOrder == rhsTypeOrder) {
                    // Same base type
                    // Both boolean type
                    // Same type order
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }
            break;
        }

        case LSS:
        case LEQ:
        case GTR:
        case GEQ: {
            if(lhsTypeCateg == DataTypeCategory::arithmetic && rhsTypeCateg == DataTypeCategory::arithmetic) {
                if(lhsTypeOrder == 1 && rhsTypeOrder == 1) {
                    // Same base type
                    // Both arithmetic type
                    // Both scalar type
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }
            break;
        }

        case EQL:
        case NEQ: {
            if(lhsTypeCateg == DataTypeCategory::arithmetic && rhsTypeCateg == DataTypeCategory::arithmetic) {
                if(lhsTypeOrder == rhsTypeOrder) {
                    // Same base type
                    // Both arithmetic type
                    // Same type order
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
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

        virtual void postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode){
            const AST::ExpressionNode *lhsExpr = binaryExpressionNode->getLeftExpression();
            const AST::ExpressionNode *rhsExpr = binaryExpressionNode->getRightExpression();
            
            int lhsDataType = lhsExpr->getExpressionType();
            int rhsDataType = rhsExpr->getExpressionType();

            bool lhsIsConst = lhsExpr->isConst();
            bool rhsIsConst = rhsExpr->isConst();

            int op = binaryExpressionNode->getOperator();

            int resultDataType = inferDataType(op, lhsDataType, rhsDataType);
            binaryExpressionNode->setExpressionType(resultDataType);
            binaryExpressionNode->setConst(lhsIsConst && rhsIsConst);
        }

        virtual void postNodeVisit(AST::IndexingNode *indexingNode){
            /*
             * - The index into a vector (e.g. 1 in foo[1]) must be in the range [0, i1] if the vector
             * has type veci. For example, the maximum index into a variable of type vec2 is 1.
             * - The result type of indexing into a vector is the base type associated with that vector’s
             * type. For example, if v has type bvec2 then v[0] has type bool.
             */
            int resultDataType = ANY_TYPE;

            const AST::IdentifierNode *identifier = indexingNode->getIdentifier();            
            int identifierDataType = identifier->getExpressionType();
            int identifierIsConst = identifier->isConst();
            // Identifier must be a vector
            int identifierTypeOrder = getDataTypeOrder(identifierDataType);
            if(identifierTypeOrder > 1) {
                const AST::IntLiteralNode *index = 
                    dynamic_cast<AST::IntLiteralNode *>(indexingNode->getIndexExpression());
                int indexVal = index->getVal();
                // Index must be valid
                if(indexVal >= 0 && indexVal < identifierTypeOrder) {
                    // The type of this indexing node is the base type for identifier node
                    int identifierBaseType = getDataTypeBaseType(identifierDataType);
                    resultDataType = identifierBaseType;
                }
            }

            indexingNode->setExpressionType(resultDataType);
            indexingNode->setConst(identifierIsConst);
        }

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
    SEMA::SymbolDeclVisitor symbolDeclVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolDeclVisitor);
    // symbolTable.printScopeLeaves();

    /* Symbol Look Up */
    SEMA::SymbolLUVisitor symbolLUVisitor(symbolTable);
    static_cast<AST::ASTNode *>(ast)->visit(symbolLUVisitor);
    // symbolTable.printSymbolReference();

    /* Type Inference */
    SEMA::TypeInferenceVisitor typeInferenceVisitor;
    static_cast<AST::ASTNode *>(ast)->visit(typeInferenceVisitor);
    ast_print(ast);

    return 1;
}