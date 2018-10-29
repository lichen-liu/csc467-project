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
        virtual void visit(IndexingNode *IndexingNode) {}
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
        virtual int getExpressionType() const = 0;            // pure virtual
        virtual void setExpressionType(int type) {}     // provide default definition
    protected:
        virtual ~ExpressionNode() {}
};

class ExpressionsNode: public Node {
    private:
        std::vector<ExpressionNode *> m_exprs;          // A list of ExpressionNodes
    public:
        ExpressionsNode(const std::vector<ExpressionNode *> &exprs):
            m_exprs(exprs) {}
        ExpressionsNode(std::vector<ExpressionNode *> &&exprs):
            m_exprs(exprs) {}
        ExpressionsNode() {}
    public:
        void pushBackExpression(ExpressionNode *expr) {
            m_exprs.push_back(expr);
        }
        const std::vector<ExpressionNode *> &getExpressions() const {
            return m_exprs;
        }
    protected:
        virtual ~ExpressionsNode() {
            for(ExpressionNode *expr: m_exprs) {
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
        void pushBackStatement(StatementNode *stmt) {
            m_statements.push_back(stmt);
        }
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
        void pushBackDeclaration(DeclarationNode *decl) {
            m_declarations.push_back(decl);
        }
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
    Node::destructNode(static_cast<Node *>(ast));
}

class PrintVisitor: public Visitor {
    public:
        virtual void visit(ExpressionsNode *expressionsNode) {
            for(ExpressionNode *expr: expressionsNode->getExpressions()) {
                expr->visit(*this);
            }
        }

        virtual void visit(UnaryExpressionNode *unaryExpressionNode) {}
        virtual void visit(BinaryExpressionNode *binaryExpressionNode) {}
        virtual void visit(IntLiteralNode *intLiteralNode) {}
        virtual void visit(FloatLiteralNode *floatLiteralNode) {}
        virtual void visit(BooleanLiteralNode *booleanLiteralNode) {}
        virtual void visit(IdentifierNode *identifierNode) {}
        virtual void visit(IndexingNode *IndexingNode) {}
        virtual void visit(FunctionNode *functionNode) {}
        virtual void visit(ConstructorNode *constructorNode) {}
        virtual void visit(StatementsNode *statementsNode) {}
        virtual void visit(DeclarationNode *declarationNode) {}
        virtual void visit(DeclarationsNode *declarationsNode) {}
        virtual void visit(IfStatementNode *ifStatementNode) {}
        virtual void visit(WhileStatementNode *whileStatementNode) {}
        virtual void visit(AssignmentNode *assignmentNode) {}
        virtual void visit(NestedScopeNode *nestedScopeNode) {}
        virtual void visit(ScopeNode *scopeNode) {}
};

void ast_print(node *ast) {
    PrintVisitor printer;
    static_cast<Node *>(ast)->visit(printer);
}
