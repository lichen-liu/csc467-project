#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define DEBUG_PRINT_TREE 0

node *ast = NULL;

//////////////////////////////////////////////////////////////////
//
// A Modern Object-Oriented Approach for AST
//
//////////////////////////////////////////////////////////////////

namespace AST{ /* START NAMESPACE */

int IntLiteralNode::getExpressionType() const {
    return INT_T;
}

int FloatLiteralNode::getExpressionType() const {
    return FLOAT_T;
}

int BooleanLiteralNode::getExpressionType() const {
    return BOOL_T;
}

class PrintVisitor: public Visitor {
    public:
        static std::string getTypeString(int type) {
            switch(type) {
                case ANY_TYPE:      return "ANY_TYPE";
                case BOOL_T:        return "bool";
                case BVEC2_T:       return "bvec2";
                case BVEC3_T:       return "bvec3";
                case BVEC4_T:       return "bvec4";
                case INT_T:         return "int";
                case IVEC2_T:       return "ivec2";
                case IVEC3_T:       return "ivec3";
                case IVEC4_T:       return "ivec4";
                case FLOAT_T:       return "float";
                case VEC2_T:        return "vec2";
                case VEC3_T:        return "vec3";
                case VEC4_T:        return "vec4";
                default:            return "ERROR";
            }
        }

        static std::string getOperatorString(int op) {
            switch(op) {
                case NOT:           return "!";
                case AND:           return "&&";
                case OR:            return "||";
                case PLUS:          return "+";
                case MINUS:         return "-";
                case TIMES:         return "*";
                case SLASH:         return "/";
                case EXP:           return "^";
                case EQL:           return "==";
                case NEQ:           return "!=";
                case LSS:           return "<";
                case LEQ:           return "<=";
                case GTR:           return ">";
                case GEQ:           return ">=";
                default:            return "Error";
            }
        }

        static std::string getIndentSpaceString(int indentSize) {
            return std::string(indentSize, ' ');
        }
    
    private:
        int indentSize = 0;
        static constexpr int indentIncr = 4;
    private:
        int getIndentSize() const { return indentSize; }
        void enterScope() { indentSize += indentIncr; }
        void exitScope() { indentSize -= indentIncr; }

    public:
        virtual void visit(ExpressionsNode *expressionsNode) {
            for(ExpressionNode *expr: expressionsNode->getExpressionList()) {
                printf(" ");
                expr->visit(*this);
            }
        }

        virtual void visit(UnaryExpressionNode *unaryExpressionNode) {
            // (UNARY type op expr)
            printf("(UNARY ");
            printf("%s ", getTypeString(unaryExpressionNode->getExpressionType()).c_str());
            printf("%s ", getOperatorString(unaryExpressionNode->getOperator()).c_str());
            unaryExpressionNode->getExpression()->visit(*this);
            printf(")");
        }

        virtual void visit(BinaryExpressionNode *binaryExpressionNode) {
            // (BINARY type op left right)
            printf("(BINARY ");
            printf("%s ", getTypeString(binaryExpressionNode->getExpressionType()).c_str());
            printf("%s ", getOperatorString(binaryExpressionNode->getOperator()).c_str());
            binaryExpressionNode->getLeftExpression()->visit(*this);
            printf(" ");
            binaryExpressionNode->getRightExpression()->visit(*this);
            printf(")");
        }

        virtual void visit(IntLiteralNode *intLiteralNode) {
            // <literal>
            printf("%d", intLiteralNode->getVal());
        }

        virtual void visit(FloatLiteralNode *floatLiteralNode) {
            // <literal>
            printf("%f", floatLiteralNode->getVal());
        }

        virtual void visit(BooleanLiteralNode *booleanLiteralNode) {
            // <literal>
            printf("%s", (booleanLiteralNode->getVal() ? "true":"false"));
        }

        virtual void visit(IdentifierNode *identifierNode) {
            // <identifier>
            printf("%s", identifierNode->getName().c_str());
        }

        virtual void visit(IndexingNode *indexingNode) {
            // (INDEX type id index)
            printf("(INDEX ");
            printf("%s ", getTypeString(indexingNode->getExpressionType()).c_str());
            indexingNode->getIdentifier()->visit(*this);
            printf(" ");
            indexingNode->getIndexExpression()->visit(*this);
            printf(")");
        }

        virtual void visit(FunctionNode *functionNode) {
            // (CALL name ...)
            printf("(CALL ");
            printf("%s", functionNode->getName().c_str());
            functionNode->getArgumentExpressions()->visit(*this);
            printf(")");
        }

        virtual void visit(ConstructorNode *constructorNode) {
            // (CALL name ...)
            printf("(CALL ");
            printf("%s", getTypeString(constructorNode->getExpressionType()).c_str());
            constructorNode->getArgumentExpressions()->visit(*this);
            printf(")");
        }

        virtual void visit(StatementsNode *statementsNode) {
            // (STATEMENTS ...)
            printf("%s", getIndentSpaceString(getIndentSize()).c_str());
            printf("(STATEMENTS\n");
            enterScope();
            for(StatementNode *stmt: statementsNode->getStatementList()) {
                printf("%s", getIndentSpaceString(getIndentSize()).c_str());
                stmt->visit(*this);
                printf("\n");
            }
            exitScope();
            printf("%s", getIndentSpaceString(getIndentSize()).c_str());
            printf(")\n");
        }

        virtual void visit(DeclarationNode *declarationNode) {
            // (DECLARATION variable-name type-name initial-value?)
            printf("(DECLARATION ");
            printf("%s ", declarationNode->getName().c_str());
            printf("%s", (declarationNode->isConst() ? "const ":""));
            printf("%s", getTypeString(declarationNode->getType()).c_str());
            if(declarationNode->getExpression() != nullptr) {
                printf(" ");
                declarationNode->getExpression()->visit(*this);
            }
            printf(")");
        }

        virtual void visit(DeclarationsNode *declarationsNode) {
            // (DECLARATIONS ...)
            printf("%s", getIndentSpaceString(getIndentSize()).c_str());
            printf("(DECLARATIONS\n");
            enterScope();
            for(DeclarationNode *decl: declarationsNode->getDeclarationList()) {
                printf("%s", getIndentSpaceString(getIndentSize()).c_str());
                decl->visit(*this);
                printf("\n");
            }
            exitScope();
            printf("%s", getIndentSpaceString(getIndentSize()).c_str());
            printf(")\n");
        }

        virtual void visit(IfStatementNode *ifStatementNode) {
            // (IF cond then-stmt else-stmt?)
            printf("(IF ");
            ifStatementNode->getConditionExpression()->visit(*this);
            printf(" ");
            ifStatementNode->getThenStatement()->visit(*this);
            if(ifStatementNode->getElseStatement() != nullptr) {
                printf(" ");
                ifStatementNode->getElseStatement()->visit(*this);
            }
            printf(")");
        }

        virtual void visit(AssignmentNode *assignmentNode) {
            // (ASSIGN type variable-name new-value)
            printf("(ASSIGN ");
            printf("%s ", getTypeString(assignmentNode->getExpressionType()).c_str());
            assignmentNode->getVariable()->visit(*this);
            printf(" ");
            assignmentNode->getExpression()->visit(*this);
            printf(")");
        }

        virtual void visit(NestedScopeNode *nestedScopeNode) {
            // (SCOPE (DECLARATIONS ...) (STATEMENTS ...))
            printf("(SCOPE\n");
            enterScope();
            nestedScopeNode->getDeclarations()->visit(*this);
            nestedScopeNode->getStatements()->visit(*this);
            exitScope();
            printf("%s", getIndentSpaceString(getIndentSize()).c_str());
            printf(")");
        }

        virtual void visit(ScopeNode *scopeNode) {
            // (SCOPE (DECLARATIONS ...) (STATEMENTS ...))
            printf("(SCOPE\n");
            enterScope();
            scopeNode->getDeclarations()->visit(*this);
            scopeNode->getStatements()->visit(*this);
            exitScope();
            printf(")\n");
        }
};

} /* END NAMESPACE */

//////////////////////////////////////////////////////////////////
//
// Interface Functions
//
//////////////////////////////////////////////////////////////////

node *ast_allocate(node_kind kind, ...) {
    va_list args;
    va_start(args, kind);

    // make the node
    AST::ASTNode *astNode = nullptr;

    switch(kind) {
        case SCOPE_NODE: {
            AST::DeclarationsNode *decls = static_cast<AST::DeclarationsNode *>(va_arg(args, AST::ASTNode *));
            AST::StatementsNode *stmts = static_cast<AST::StatementsNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::ScopeNode(decls, stmts);
            break;
        }

        case EXPRESSION_NODE: {
            /* Virtual ASTNode, Does not Have Instance */
            astNode = nullptr;
            break;
        }

        case EXPRESSIONS_NODE: {
            AST::ExpressionsNode *exprs = static_cast<AST::ExpressionsNode *>(va_arg(args, AST::ASTNode *));
            AST::ExpressionNode *expr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            if(exprs == nullptr) {
                exprs = new AST::ExpressionsNode();
            }
            if(expr != nullptr) {
                exprs->pushBackExpression(expr);
            }
            astNode = exprs;
            break;
        }

        case UNARY_EXPRESION_NODE: {
            int op = va_arg(args, int);
            AST::ExpressionNode *expr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::UnaryExpressionNode(op, expr);
            break;
        }

        case BINARY_EXPRESSION_NODE: {
            int op = va_arg(args, int);
            AST::ExpressionNode *leftExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            AST::ExpressionNode *rightExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::BinaryExpressionNode(op, leftExpr, rightExpr);
            break;
        }

        case INT_C_NODE: {
            int val = va_arg(args, int);
            astNode = new AST::IntLiteralNode(val);
            break;
        }

        case FLOAT_C_NODE: {
            double val = va_arg(args, double);
            astNode = new AST::FloatLiteralNode(val);
            break;
        }

        case BOOL_C_NODE: {
            bool val = static_cast<bool>(va_arg(args, int));
            astNode = new AST::BooleanLiteralNode(val);
            break;
        }

        case VAR_NODE: {
            /* Virtual ASTNode, Does not Have Instance */
            astNode = nullptr;
            break;
        }

        case ID_NODE: {
            const char *id = va_arg(args, char *);
            astNode = new AST::IdentifierNode(id);
            break;
        }

        case INDEXING_NODE: {
            /* 
             * Stick to http://dsrg.utoronto.ca/csc467/lab/lab3.pdf
             * Use expression as index.
             * Need another layer to convert IntLiteralNode to ExpressionNode.
             */
            const char *id = va_arg(args, char *);
            AST::ExpressionNode *indexExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::IndexingNode(new AST::IdentifierNode(id), indexExpr);
            break;
        }

        case FUNCTION_NODE: {
            const char *functionName = va_arg(args, char *);
            AST::ExpressionsNode *argExprs = static_cast<AST::ExpressionsNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::FunctionNode(functionName, argExprs);
            break;
        }

        case CONSTRUCTOR_NODE: {
            int type = va_arg(args, int);
            AST::ExpressionsNode *argExprs = static_cast<AST::ExpressionsNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::ConstructorNode(type, argExprs);
            break;
        }

        case STATEMENT_NODE: {
            /* Virtual ASTNode, Does not Have Instance */
            astNode = nullptr;
            break;
        }

        case IF_STATEMENT_NODE: {
            AST::ExpressionNode *condExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            AST::StatementNode *thenStmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
            AST::StatementNode *elseStmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::IfStatementNode(condExpr, thenStmt, elseStmt);
            break;
        }

        case WHILE_STATEMENT_NODE: {
            AST::ExpressionNode *condExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            AST::StatementNode *bodyStmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::WhileStatementNode(condExpr, bodyStmt);
            break;
        }

        case ASSIGNMENT_NODE: {
            AST::VariableNode *var = static_cast<AST::VariableNode *>(va_arg(args, AST::ASTNode *));
            AST::ExpressionNode *newValExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::AssignmentNode(var, newValExpr);
            break;
        }

        case NESTED_SCOPE_NODE: {
            /* Argument ScopeNode is destructed and converted to NestedScopeNode */
            AST::ScopeNode *scopeNode = static_cast<AST::ScopeNode *>(va_arg(args, AST::ASTNode *));
            astNode = AST::ScopeNode::convertToNestedScopeNode(scopeNode);
            break;
        }

        case STALL_STATEMENT_NODE: {
            astNode = new AST::StallStatementNode();
            break;
        }

        case STATEMENTS_NODE: {
            AST::StatementsNode *stmts = static_cast<AST::StatementsNode *>(va_arg(args, AST::ASTNode *));
            AST::StatementNode *stmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
            if(stmts == nullptr) {
                stmts = new AST::StatementsNode();
            }
            if(stmt != nullptr) {
                stmts->pushBackStatement(stmt);
            }
            astNode = stmts;
            break;
        }

        case DECLARATION_NODE: {
            const char *varName = va_arg(args, char *);
            int isConst = va_arg(args, int);
            int type = va_arg(args, int);
            AST::ExpressionNode *initValExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
            astNode = new AST::DeclarationNode(varName, static_cast<bool>(isConst), type, initValExpr);
            break;
        }

        case DECLARATIONS_NODE: {
            AST::DeclarationsNode *decls = static_cast<AST::DeclarationsNode *>(va_arg(args, AST::ASTNode *));
            AST::DeclarationNode *decl = static_cast<AST::DeclarationNode *>(va_arg(args, AST::ASTNode *));
            if(decls == nullptr) {
                decls = new AST::DeclarationsNode();
            }
            if(decl != nullptr) {
                decls->pushBackDeclaration(decl);
            }
            astNode = decls;
            break;
        }

        case UNKNOWN:
        default:
            astNode = nullptr;
            break;
    }

    va_end(args);

    return static_cast<node *>(astNode);
}

void ast_free(node *ast) {
    if(ast != nullptr) {
        AST::ASTNode::destructNode(static_cast<AST::ASTNode *>(ast));
    }
}

void ast_print(node *ast) {
    if(ast != nullptr) {
        AST::PrintVisitor printer;
        static_cast<AST::ASTNode *>(ast)->visit(printer);
    }
}
