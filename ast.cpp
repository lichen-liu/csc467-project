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
class Node {
    public:
        virtual void visit(Visitor &vistor) = 0;
    protected:
        virtual ~Node() {}
    public:
        /* The only way to destruct any AST Node */
        static void destructNode(Node *astNode) { delete astNode; }
};

class ExpressionNode: public Node {
    /* Pure Virtual Intermediate Layer */
    public:
        virtual int getExpressionType() const = 0;      // pure virtual
        virtual void setExpressionType(int type) {}     // provide default definition
    protected:
        virtual ~ExpressionNode() {}
};

class ExpressionsNode: public Node {
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
                Node::destructNode(expr);
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
            Node::destructNode(m_expr);
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
            Node::destructNode(m_leftExpr);
            Node::destructNode(m_rightExpr);
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
            Node::destructNode(m_identifier);
            Node::destructNode(m_indexExpr);
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
            Node::destructNode(m_argExprs);
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
            Node::destructNode(m_argExprs);
        }

    VISIT_THIS_NODE
};

class StatementNode: public Node {
    /* Pure Virtual Intermediate Layer */
    protected:
        virtual ~StatementNode() {}
};

class StatementsNode: public Node {
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
                Node::destructNode(stmt);
            }
        }

    VISIT_THIS_NODE
};

class DeclarationNode: public Node {
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
                Node::destructNode(m_initValExpr);
            }
        }
    
    VISIT_THIS_NODE
};

class DeclarationsNode: public Node {
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
                Node::destructNode(decl);
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
            Node::destructNode(m_condExpr);
            Node::destructNode(m_thenStmt);
            if(m_elseStmt != nullptr) {
                Node::destructNode(m_elseStmt);
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
            Node::destructNode(m_condExpr);
            Node::destructNode(m_bodyStmt);
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
            Node::destructNode(m_var);
            Node::destructNode(m_newValExpr);
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
            Node::destructNode(m_decls);
            Node::destructNode(m_stmts);
        }

    VISIT_THIS_NODE
};

/* Global ScopeNode */
class ScopeNode: public Node {
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
                Node::destructNode(m_decls);
            }
            if(m_stmts != nullptr) {
                Node::destructNode(m_stmts);
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

//////////////////////////////////////////////////////////////////
//
// Interface Functions
//
//////////////////////////////////////////////////////////////////

node *ast_allocate(node_kind kind, ...) {
    va_list args;

    // make the node
    Node *astNode = nullptr;

    switch(kind) {
        case SCOPE_NODE: {
            DeclarationsNode *decls = static_cast<DeclarationsNode *>(va_arg(args, Node *));
            StatementsNode *stmts = static_cast<StatementsNode *>(va_arg(args, Node *));
            astNode = new ScopeNode(decls, stmts);
            break;
        }

        case EXPRESSION_NODE: {
            /* Virtual Node, Does not Have Instance */
            astNode = nullptr;
            break;
        }

        case EXPRESSIONS_NODE: {
            ExpressionsNode *exprs = static_cast<ExpressionsNode *>(va_arg(args, Node *));
            ExpressionNode *expr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            if(exprs == nullptr) {
                exprs = new ExpressionsNode();
            }
            if(expr != nullptr) {
                exprs->pushBackExpression(expr);
            }
            astNode = exprs;
            break;
        }

        case UNARY_EXPRESION_NODE: {
            int op = va_arg(args, int);
            ExpressionNode *expr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            astNode = new UnaryExpressionNode(op, expr);
            break;
        }

        case BINARY_EXPRESSION_NODE: {
            int op = va_arg(args, int);
            ExpressionNode *leftExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            ExpressionNode *rightExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            astNode = new BinaryExpressionNode(op, leftExpr, rightExpr);
            break;
        }

        case INT_C_NODE: {
            int val = va_arg(args, int);
            astNode = new IntLiteralNode(val);
            break;
        }

        case FLOAT_C_NODE: {
            double val = va_arg(args, double);
            astNode = new FloatLiteralNode(val);
            break;
        }

        case BOOL_C_NODE: {
            bool val = static_cast<bool>(va_arg(args, int));
            astNode = new BooleanLiteralNode(val);
            break;
        }

        case VAR_NODE: {
            /* Virtual Node, Does not Have Instance */
            astNode = nullptr;
            break;
        }

        case ID_NODE: {
            const char *id = va_arg(args, char *);
            astNode = new IdentifierNode(id);
            break;
        }

        case INDEXING_NODE: {
            /* 
             * Stick to http://dsrg.utoronto.ca/csc467/lab/lab3.pdf
             * Use expression as index.
             * Need another layer to convert IntLiteralNode to ExpressionNode.
             */
            const char *id = va_arg(args, char *);
            ExpressionNode *indexExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            astNode = new IndexingNode(new IdentifierNode(id), indexExpr);
            break;
        }

        case FUNCTION_NODE: {
            const char *functionName = va_arg(args, char *);
            ExpressionsNode *argExprs = static_cast<ExpressionsNode *>(va_arg(args, Node *));
            astNode = new FunctionNode(functionName, argExprs);
            break;
        }

        case CONSTRUCTOR_NODE: {
            int type = va_arg(args, int);
            ExpressionsNode *argExprs = static_cast<ExpressionsNode *>(va_arg(args, Node *));
            astNode = new ConstructorNode(type, argExprs);
            break;
        }

        case STATEMENT_NODE: {
            /* Virtual Node, Does not Have Instance */
            astNode = nullptr;
            break;
        }

        case IF_STATEMENT_NODE: {
            ExpressionNode *condExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            StatementNode *thenStmt = static_cast<StatementNode *>(va_arg(args, Node *));
            StatementNode *elseStmt = static_cast<StatementNode *>(va_arg(args, Node *));
            astNode = new IfStatementNode(condExpr, thenStmt, elseStmt);
            break;
        }

        case WHILE_STATEMENT_NODE: {
            ExpressionNode *condExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            StatementNode *bodyStmt = static_cast<StatementNode *>(va_arg(args, Node *));
            astNode = new WhileStatementNode(condExpr, bodyStmt);
            break;
        }

        case ASSIGNMENT_NODE: {
            VariableNode *var = static_cast<VariableNode *>(va_arg(args, Node *));
            ExpressionNode *newValExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            astNode = new AssignmentNode(var, newValExpr);
            break;
        }

        case NESTED_SCOPE_NODE: {
            /* Argument ScopeNode is destructed and converted to NestedScopeNode */
            ScopeNode *scopeNode = static_cast<ScopeNode *>(va_arg(args, Node *));
            astNode = ScopeNode::convertToNestedScopeNode(scopeNode);
            break;
        }

        case STALL_STATEMENT_NODE: {
            astNode = new StallStatementNode();
            break;
        }

        case STATEMENTS_NODE: {
            StatementsNode *stmts = static_cast<StatementsNode *>(va_arg(args, Node *));
            StatementNode *stmt = static_cast<StatementNode *>(va_arg(args, Node *));
            if(stmts == nullptr) {
                stmts = new StatementsNode();
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
            ExpressionNode *initValExpr = static_cast<ExpressionNode *>(va_arg(args, Node *));
            astNode = new DeclarationNode(varName, static_cast<bool>(isConst), type, initValExpr);
            break;
        }

        case DECLARATIONS_NODE: {
            DeclarationsNode *decls = static_cast<DeclarationsNode *>(va_arg(args, Node *));
            DeclarationNode *decl = static_cast<DeclarationNode *>(va_arg(args, Node *));
            if(decls == nullptr) {
                decls = new DeclarationsNode();
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

    return astNode;
}

void ast_free(node *ast) {
    if(ast != nullptr) {
        Node::destructNode(static_cast<Node *>(ast));
    }
}

#define STRINGIFY(x) #x
class PrintVisitor: public Visitor {
    public:
        static std::string getTypeString(int type) {
            switch(type) {
                case ANY_TYPE:      return STRINGIFY(ANY_TYPE);
                case BOOL_T:        return STRINGIFY(BOOL_T);
                case BVEC2_T:       return STRINGIFY(BVEC2_T);
                case BVEC3_T:       return STRINGIFY(BVEC3_T);
                case BVEC4_T:       return STRINGIFY(BVEC4_T);
                case INT_T:         return STRINGIFY(INT_T);
                case IVEC2_T:       return STRINGIFY(IVEC2_T);
                case IVEC3_T:       return STRINGIFY(IVEC3_T);
                case IVEC4_T:       return STRINGIFY(IVEC4_T);
                case FLOAT_T:       return STRINGIFY(FLOAT_T);
                case VEC2_T:        return STRINGIFY(VEC2_T);
                case VEC3_T:        return STRINGIFY(VEC3_T);
                case VEC4_T:        return STRINGIFY(VEC4_T);
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
            printf("    (STATEMENTS\n");
            for(StatementNode *stmt: statementsNode->getStatementList()) {
                printf("        ");
                stmt->visit(*this);
                printf("\n");
            }
            printf("    )\n");
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
            printf("    (DECLARATIONS\n");
            for(DeclarationNode *decl: declarationsNode->getDeclarationList()) {
                printf("        ");
                decl->visit(*this);
                printf("\n");
            }
            printf("    )\n");
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
            printf("( ");
            printf("%s ", getTypeString(assignmentNode->getExpressionType()).c_str());
            assignmentNode->getVariable()->visit(*this);
            printf(" ");
            assignmentNode->getExpression()->visit(*this);
            printf(")");
        }

        virtual void visit(NestedScopeNode *nestedScopeNode) {
            // (SCOPE (DECLARATIONS ...) (STATEMENTS ...))
            printf("(SCOPE\n");
            nestedScopeNode->getDeclarations()->visit(*this);
            nestedScopeNode->getStatements()->visit(*this);
            printf(")\n");
        }

        virtual void visit(ScopeNode *scopeNode) {
            // (SCOPE (DECLARATIONS ...) (STATEMENTS ...))
            printf("(SCOPE\n");
            scopeNode->getDeclarations()->visit(*this);
            scopeNode->getStatements()->visit(*this);
            printf(")\n");
        }
};

void ast_print(node *ast) {
    if(ast != nullptr) {
        PrintVisitor printer;
        static_cast<Node *>(ast)->visit(printer);
    }
}
