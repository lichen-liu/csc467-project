#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

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
#define ANY_TYPE -1

namespace AST{ /* START NAMESPACE */

class Visitor;

class ExpressionNode;
class ExpressionsNode;
class UnaryExpressionNode;
class BinaryExpressionNode;
class IntLiteralNode;
class FloatLiteralNode;
class BooleanLiteralNode;
class VariableNode;
class IdentifierNode;
class IndexingNode;
class FunctionNode;
class ConstructorNode;
class StatementNode;
class StatementsNode;
class DeclarationNode;
class DeclarationsNode;
class IfStatementNode;
class WhileStatementNode;
class AssignmentNode;
class StallStatementNode;
class NestedScopeNode;
class ScopeNode;

class Visitor {
    public:
        virtual void visit(ExpressionNode *expressionNode) {}
        virtual void visit(ExpressionsNode *expressionsNode) {}
        virtual void visit(UnaryExpressionNode *unaryExpressionNode) {}
        virtual void visit(BinaryExpressionNode *binaryExpressionNode) {}
        virtual void visit(IntLiteralNode *intLiteralNode) {}
        virtual void visit(FloatLiteralNode *floatLiteralNode) {}
        virtual void visit(BooleanLiteralNode *booleanLiteralNode) {}
        virtual void visit(VariableNode *variableNode) {}
        virtual void visit(IdentifierNode *identifierNode) {}
        virtual void visit(IndexingNode *indexingNode) {}
        virtual void visit(FunctionNode *functionNode) {}
        virtual void visit(ConstructorNode *constructorNode) {}
        virtual void visit(StatementNode *statementNode) {}
        virtual void visit(StatementsNode *statementsNode) {}
        virtual void visit(DeclarationNode *declarationNode) {}
        virtual void visit(DeclarationsNode *declarationsNode) {}
        virtual void visit(IfStatementNode *ifStatementNode) {}
        virtual void visit(WhileStatementNode *whileStatementNode) {}
        virtual void visit(AssignmentNode *assignmentNode) {}
        virtual void visit(StallStatementNode *stallStatementNode) {}
        virtual void visit(NestedScopeNode *nestedScopeNode) {}
        virtual void visit(ScopeNode *scopeNode) {}
};

#define VISIT_THIS_NODE     public:                                             \
                            virtual void visit(Visitor &visitor) {              \
                                visitor.visit(this);                            \
                            }

/*
 * Base class for AST Nodes
 */
class ASTNode {
    public:
        virtual void visit(Visitor &vistor) = 0;
    protected:
        virtual ~ASTNode() {}
    public:
        /* The only way to destruct any AST ASTNode */
        static void destructNode(ASTNode *astNode) { delete astNode; }
};

class ExpressionNode: public ASTNode {
    /* Pure Virtual Intermediate Layer */
    public:
        virtual int getExpressionType() const = 0;      // pure virtual
        virtual void setExpressionType(int type) {}     // provide default definition
    protected:
        virtual ~ExpressionNode() {}
};

class ExpressionsNode: public ASTNode {
    private:
        std::vector<ExpressionNode *> m_expressions;    // A list of ExpressionNodes
    public:
        ExpressionsNode(const std::vector<ExpressionNode *> &expressions):
            m_expressions(expressions) {}
        ExpressionsNode(std::vector<ExpressionNode *> &&expressions):
            m_expressions(expressions) {}
        ExpressionsNode() {}
    public:
        void pushBackExpression(ExpressionNode *expr) { m_expressions.push_back(expr); }
        const std::vector<ExpressionNode *> &getExpressionList() const { return m_expressions; }
    protected:
        virtual ~ExpressionsNode() {
            for(ExpressionNode *expr: m_expressions) {
                ASTNode::destructNode(expr);
            }
        }

    VISIT_THIS_NODE
};

class UnaryExpressionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        int m_op;                                       // unary operators defined in parser.tab.h
        ExpressionNode *m_expr;                         // sub-expression
    public:
        UnaryExpressionNode(int op, ExpressionNode *expr):
            m_op(op), m_expr(expr) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        int getOperator() const { return m_op; }
        ExpressionNode *getExpression() const { return m_expr; }
    protected:
        virtual ~UnaryExpressionNode() {
            ASTNode::destructNode(m_expr);
        }
    
    VISIT_THIS_NODE
};

class BinaryExpressionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        int m_op;                                       // binary operators defined in parser.tab.h
        ExpressionNode *m_leftExpr;                     // left sub-expression
        ExpressionNode *m_rightExpr;                    // right sub-expression
    public:
        BinaryExpressionNode(int op, ExpressionNode *leftExpr, ExpressionNode *rightExpr):
            m_op(op), m_leftExpr(leftExpr), m_rightExpr(rightExpr) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        int getOperator() const { return m_op; }
        ExpressionNode *getLeftExpression() const { return m_leftExpr; }
        ExpressionNode *getRightExpression() const { return m_rightExpr; }
    protected:
        virtual ~BinaryExpressionNode() {
            ASTNode::destructNode(m_leftExpr);
            ASTNode::destructNode(m_rightExpr);
        }
    
    VISIT_THIS_NODE
};

class IntLiteralNode: public ExpressionNode {
    private:
        int m_val;                                      // value of this int literal
    public:
        IntLiteralNode(int val):
            m_val(val) {}
    public:
        virtual int getExpressionType() const { return INT_T; }
    public:
        int getVal() const { return m_val; }
    protected:
        virtual ~IntLiteralNode() {}
    
    VISIT_THIS_NODE
};

class FloatLiteralNode: public ExpressionNode {
    private:
        float m_val;                                    // value of this float literal
    public:
        FloatLiteralNode(float val):
            m_val(val) {}
    public:
        virtual int getExpressionType() const { return FLOAT_T; }
    public:
        float getVal() const { return m_val; }
    protected:
        virtual ~FloatLiteralNode() {}

    VISIT_THIS_NODE
};

class BooleanLiteralNode: public ExpressionNode {
    private:
        bool m_val;                                     // value of this boolean literal
    public:
        BooleanLiteralNode(bool val):
            m_val(val) {}
    public:
        virtual int getExpressionType() const { return BOOL_T; }
    public:
        bool getVal() const { return m_val; }
    protected:
        virtual ~BooleanLiteralNode() {}

    VISIT_THIS_NODE
};

class VariableNode: public ExpressionNode {
    /* Pure Virtual Intermediate Layer */
};

class IdentifierNode: public VariableNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::string m_id;                               // name of this identifier
    public:
        IdentifierNode(const std::string &id):
            m_id(id) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        const std::string &getName() const { return m_id; }
    protected:
        virtual ~IdentifierNode() {}

    VISIT_THIS_NODE
};

class IndexingNode: public VariableNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        IdentifierNode *m_identifier;                   // identifier node of the vector variable
        ExpressionNode *m_indexExpr;                    // index expression node
    public:
        IndexingNode(IdentifierNode *identifier, ExpressionNode *indexExpr):
            m_identifier(identifier), m_indexExpr(indexExpr) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        IdentifierNode *getIdentifier() const { return m_identifier; }
        ExpressionNode *getIndexExpression() const { return m_indexExpr; }
    protected:
        virtual ~IndexingNode() {
            ASTNode::destructNode(m_identifier);
            ASTNode::destructNode(m_indexExpr);
        }

    VISIT_THIS_NODE
};

class FunctionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::string m_functionName;                     // name of this function
        ExpressionsNode *m_argExprs;                    // argument expressions of this function
    public:
        FunctionNode(const std::string &functionName, ExpressionsNode *argExprs):
            m_functionName(functionName), m_argExprs(argExprs) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        const std::string &getName() const { return m_functionName; }
        ExpressionsNode *getArgumentExpressions() const { return m_argExprs; }
    protected:
        virtual ~FunctionNode() {
            ASTNode::destructNode(m_argExprs);
        }

    VISIT_THIS_NODE
};

class ConstructorNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        ExpressionsNode *m_argExprs;                    // argument expressions of this constructor
    public:
        ConstructorNode(int type, ExpressionsNode *argExprs):
            m_type(type), m_argExprs(argExprs) {}
    public:
        virtual int getExpressionType() const { return m_type; }
    public:
        ExpressionsNode *getArgumentExpressions() const { return m_argExprs; }
    protected:
        virtual ~ConstructorNode() {
            ASTNode::destructNode(m_argExprs);
        }

    VISIT_THIS_NODE
};

class StatementNode: public ASTNode {
    /* Pure Virtual Intermediate Layer */
    protected:
        virtual ~StatementNode() {}
};

class StatementsNode: public ASTNode {
    private:
        std::vector<StatementNode *> m_statements;      // a list of StatementNodes
    public:
        StatementsNode(const std::vector<StatementNode *> &statements):
            m_statements(statements) {}
        StatementsNode(std::vector<StatementNode *> &&statements):
            m_statements(statements) {}
        StatementsNode() {}
    public:
        void pushBackStatement(StatementNode *stmt) { m_statements.push_back(stmt); }
        const std::vector<StatementNode *> &getStatementList() const { return m_statements; }
    protected:
        virtual ~StatementsNode() {
            for(StatementNode *stmt: m_statements) {
                ASTNode::destructNode(stmt);
            }
        }

    VISIT_THIS_NODE
};

class DeclarationNode: public ASTNode {
    private:
        // not using IdentifierNode because the name itself is not yet an expression
        std::string m_variableName;                     // variable name
        bool m_isConst;                                 // const qualified
        int m_type;                                     // types defined in parser.tab.h
        ExpressionNode *m_initValExpr = nullptr;        // initial value expression (optional)
    public:
        DeclarationNode(const std::string &variableName, bool isConst, int type, ExpressionNode *initValExpr = nullptr):
            m_variableName(variableName), m_isConst(isConst), m_type(type), m_initValExpr(initValExpr) {}
    public:
        const std::string &getName() const { return m_variableName; }
        bool isConst() const { return m_isConst; }
        int getType() const { return m_type; }
        ExpressionNode *getExpression() const { return m_initValExpr; }
    protected:
        virtual ~DeclarationNode() {
            if(m_initValExpr != nullptr) {
                ASTNode::destructNode(m_initValExpr);
            }
        }
    
    VISIT_THIS_NODE
};

class DeclarationsNode: public ASTNode {
    private:
        std::vector<DeclarationNode *> m_declarations;  // a list of DeclarationNodes
    public:
        DeclarationsNode(const std::vector<DeclarationNode *> &declarations):
            m_declarations(declarations) {}
        DeclarationsNode(std::vector<DeclarationNode *> &&declarations):
            m_declarations(declarations) {}
        DeclarationsNode() {}
    public:
        void pushBackDeclaration(DeclarationNode *decl) { m_declarations.push_back(decl); }
        const std::vector<DeclarationNode *> &getDeclarationList() const { return m_declarations; }
    protected:
        virtual ~DeclarationsNode() {
            for(DeclarationNode *decl: m_declarations) {
                ASTNode::destructNode(decl);
            }
        }

    VISIT_THIS_NODE
};

class IfStatementNode: public StatementNode {
    private:
        ExpressionNode *m_condExpr;                     // condition expression
        StatementNode *m_thenStmt;                      // then statement
        StatementNode *m_elseStmt = nullptr;            // else statement (optional)
    public:
        IfStatementNode(ExpressionNode *condExpr, StatementNode *thenStmt, StatementNode *elseStmt = nullptr):
            m_condExpr(condExpr), m_thenStmt(thenStmt), m_elseStmt(elseStmt) {}
    public:
        ExpressionNode *getConditionExpression() const { return m_condExpr; }
        StatementNode *getThenStatement() const { return m_thenStmt; }
        StatementNode *getElseStatement() const { return m_elseStmt; }
    protected:
        virtual ~IfStatementNode() {
            ASTNode::destructNode(m_condExpr);
            ASTNode::destructNode(m_thenStmt);
            if(m_elseStmt != nullptr) {
                ASTNode::destructNode(m_elseStmt);
            }
        }
    
    VISIT_THIS_NODE
};

class WhileStatementNode: public StatementNode {
    /* No Support */
    private:
        ExpressionNode *m_condExpr;                     // condition expression
        StatementNode *m_bodyStmt;                      // loop body statement
    public:
        WhileStatementNode(ExpressionNode *condExpr, StatementNode *bodyStmt):
            m_condExpr(condExpr), m_bodyStmt(bodyStmt) {}
    protected:
        virtual ~WhileStatementNode() {
            ASTNode::destructNode(m_condExpr);
            ASTNode::destructNode(m_bodyStmt);
        }
    
    VISIT_THIS_NODE
};

class AssignmentNode: public StatementNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        VariableNode *m_var;                            // variable node
        ExpressionNode *m_newValExpr;                   // new value expression
    public:
        AssignmentNode(VariableNode *var, ExpressionNode *newValExpr):
            m_var(var), m_newValExpr(newValExpr) {}
    public:
        int getExpressionType() const { return m_type; }
        void setExpressionType(int type) { m_type = type; }
        VariableNode *getVariable() const { return m_var; }
        ExpressionNode *getExpression() const { return m_newValExpr; }
    protected:
        virtual ~AssignmentNode() {
            ASTNode::destructNode(m_var);
            ASTNode::destructNode(m_newValExpr);
        }
    
    VISIT_THIS_NODE
};

/* In case of a single semicolon statement */
class StallStatementNode: public StatementNode {
    public:
        StallStatementNode() {}
    protected:
        virtual ~StallStatementNode() {}

    VISIT_THIS_NODE
};

/* Inner ScopeNode */
class NestedScopeNode: public StatementNode {
    private:
        DeclarationsNode *m_decls;
        StatementsNode *m_stmts;
    public:
        NestedScopeNode(DeclarationsNode *decls, StatementsNode *stmts):
            m_decls(decls), m_stmts(stmts) {}
    public:
        DeclarationsNode *getDeclarations() const { return m_decls; }
        StatementsNode *getStatements() const { return m_stmts; }
    protected:
        virtual ~NestedScopeNode() {
            ASTNode::destructNode(m_decls);
            ASTNode::destructNode(m_stmts);
        }

    VISIT_THIS_NODE
};

/* Global ScopeNode */
class ScopeNode: public ASTNode {
    private:
        DeclarationsNode *m_decls;
        StatementsNode *m_stmts;
    public:
        ScopeNode(DeclarationsNode *decls, StatementsNode *stmts):
            m_decls(decls), m_stmts(stmts) {}
    public:
        DeclarationsNode *getDeclarations() const { return m_decls; }
        StatementsNode *getStatements() const { return m_stmts; }
    protected:
        virtual ~ScopeNode() {
            if(m_decls != nullptr) {
                ASTNode::destructNode(m_decls);
            }
            if(m_stmts != nullptr) {
                ASTNode::destructNode(m_stmts);
            }
        }
    
    public:
        static NestedScopeNode *convertToNestedScopeNode(ScopeNode *scopeNode) {
            NestedScopeNode *nestedScopeNode = new NestedScopeNode(scopeNode->m_decls, scopeNode->m_stmts);
            
            // Delete scopeNode
            scopeNode->m_decls = nullptr;
            scopeNode->m_stmts = nullptr;
            delete scopeNode;

            return nestedScopeNode;
        }

    VISIT_THIS_NODE
};

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
