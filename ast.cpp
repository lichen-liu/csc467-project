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

class ScopeNode;
class ExpressionNode;
class UnaryExpressionNode;
class BinaryExpressionNode;
class IntLiteralNode;
class FloatLiteralNode;
class BooleanLiteralNode;
class IdentifierNode;
class VariableNode; // maybe change to refer to indexing vector
class FunctionNode;
class ConstructorNode;
class StatementNode;
class StatementsNode;
class IfStatementNode;
class WhileStatementNode;
class AssignmentNode;
class NestedScopeNode;
class DeclarationNode;
class DeclarationsNode;

class Visitor {
    public:
        void visit(ScopeNode *scopeNode);
        void visit(ExpressionNode *expressionNode);
        void visit(UnaryExpressionNode *unaryExpressionNode);
        void visit(BinaryExpressionNode *binaryExpressionNode);
        void visit(IntLiteralNode *intLiteralNode);
        void visit(FloatLiteralNode *floatLiteralNode);
        void visit(BooleanLiteralNode *booleanLiteralNode);
        void visit(IdentifierNode *identifierNode);
        void visit(VariableNode *variableNode);
        void visit(FunctionNode *functionNode);
        void visit(ConstructorNode *constructorNode);
        void visit(StatementNode *statementNode);
        void visit(StatementsNode *statementsNode);
        void visit(IfStatementNode *ifStatementNode);
        void visit(WhileStatementNode *whileStatementNode);
        void visit(AssignmentNode *assignmentNode);
        void visit(NestedScopeNode *nestedScopeNode);
        void visit(DeclarationNode *declarationNode);
        void visit(DeclarationsNode *declarationsNode);
};

class PrintVisitor {
    public:
        void print(ScopeNode *scopeNode);
        void print(ExpressionNode *expressionNode);
        void print(UnaryExpressionNode *unaryExpressionNode);
        void print(BinaryExpressionNode *binaryExpressionNode);
        void print(IntLiteralNode *intLiteralNode);
        void print(FloatLiteralNode *floatLiteralNode);
        void print(BooleanLiteralNode *booleanLiteralNode);
        void print(IdentifierNode *identifierNode);
        void print(VariableNode *variableNode);
        void print(FunctionNode *functionNode);
        void print(ConstructorNode *constructorNode);
        void print(StatementNode *statementNode);
        void print(StatementsNode *statementsNode);
        void print(IfStatementNode *ifStatementNode);
        void print(WhileStatementNode *whileStatementNode);
        void print(AssignmentNode *assignmentNode);
        void print(NestedScopeNode *nestedScopeNode);
        void print(DeclarationNode *declarationNode);
        void print(DeclarationsNode *declarationsNode);
};

#define VISIT_THIS_NODE     virtual void visit(Visitor &visitor) {              \
                                visitor.visit(this);                            \
                            }

#define PRINT_THIS_NODE     virtual void print(PrintVisitor &printVisitor) {    \
                                printVisitor.print(this);                       \
                            }
            
#define ACT_ON_THIS_NODE    public:                                             \
                            VISIT_THIS_NODE                                     \
                            PRINT_THIS_NODE

class ScopeNode: public Node {
    private:
        DeclarationsNode *m_declarationsNode;
        StatementsNode *m_statementsNode;
    public:
        ScopeNode(DeclarationsNode *declarationsNode, StatementsNode *statementsNode):
            m_declarationsNode(declarationsNode), m_statementsNode(statementsNode) {}

    ACT_ON_THIS_NODE
};

class ExpressionNode: public Node {
    /* Pure Virtual Intermediate Layer */
    public:
        virtual int getExpressionType() = 0;            // pure virtual
        virtual void setExpressionType(int type) {}     // provide default definition
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
        virtual int getExpressionType() { return m_type; }
    
    ACT_ON_THIS_NODE
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
        virtual int getExpressionType() { return m_type; }
    
    ACT_ON_THIS_NODE
};

class IntLiteralNode: public ExpressionNode {
    private:
        int m_val;                                      // value of this int literal
    public:
        IntLiteralNode(int val):
            m_val(val) {}
    public:
        virtual int getExpressionType() { return INT_T; }
    
    ACT_ON_THIS_NODE
};

class FloatLiteralNode: public ExpressionNode {
    private:
        float m_val;                                    // value of this float literal
    public:
        FloatLiteralNode(float val):
            m_val(val) {}
    public:
        virtual int getExpressionType() { return FLOAT_T; }
    
    ACT_ON_THIS_NODE
};

class BooleanLiteralNode: public ExpressionNode {
    private:
        bool m_val;                                     // value of this boolean literal
    public:
        BooleanLiteralNode(bool val):
            m_val(val) {}
    public:
        virtual int getExpressionType() { return BOOL_T; }

    ACT_ON_THIS_NODE
};

class IdentifierNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::string m_id;                               // name of this identifier
    public:
        IdentifierNode(const std::string &id):
            m_id(id) {}
    public:
        virtual int getExpressionType() { return m_type; }

    ACT_ON_THIS_NODE
};

class VariableNode: public ExpressionNode {             // maybe change to refer to indexing vector
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        IdentifierNode *m_identifierNode;               // identifier node of the vector variable
        ExpressionNode *m_indexExprNode;                // index expression node
    public:
        VariableNode(IdentifierNode *identifierNode, ExpressionNode *indexExprNode):
            m_identifierNode(identifierNode), m_indexExprNode(indexExprNode) {}
    public:
        virtual int getExpressionType() { return m_type; }

    ACT_ON_THIS_NODE
};

class FunctionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::string m_functionName;                     // name of this function
        std::vector<ExpressionNode *> m_arguments;      // argument expressions of this function
    public:
        FunctionNode(const std::string &functionName, const std::vector<ExpressionNode *> &arguments):
            m_functionName(functionName), m_arguments(arguments) {}
        FunctionNode(const std::string &functionName, std::vector<ExpressionNode *> &&arguments):
            m_functionName(functionName), m_arguments(arguments) {}
    public:
        virtual int getExpressionType() { return m_type; }

    ACT_ON_THIS_NODE
};

class ConstructorNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::vector<ExpressionNode *> m_arguments;      // argument expressions of this constructor
    public:
        ConstructorNode(int type, const std::vector<ExpressionNode *> &arguments):
            m_type(type), m_arguments(arguments) {}
        ConstructorNode(int type, std::vector<ExpressionNode *> &&arguments):
            m_type(type), m_arguments(arguments) {}
    public:
        virtual int getExpressionType() { return m_type; }

    ACT_ON_THIS_NODE
};

// class StatementNode;
// class StatementsNode;
// class IfStatementNode;
// class WhileStatementNode;
// class AssignmentNode;
// class NestedScopeNode;
// class DeclarationNode;
// class DeclarationsNode;

//////////////////////////////////////////////////////////////////
//
// Interface Functions
//
//////////////////////////////////////////////////////////////////

node *ast_allocate(node_kind kind, ...) {
  va_list args;

  // make the node
  node *ast = (node *) malloc(sizeof(node));
  memset(ast, 0, sizeof *ast);
  ast->kind = kind;

  va_start(args, kind); 

  switch(kind) {
  
  // ...

  case BINARY_EXPRESSION_NODE:
    ast->binary_expr.op = va_arg(args, int);
    ast->binary_expr.left = va_arg(args, node *);
    ast->binary_expr.right = va_arg(args, node *);
    break;

  // ...

  default: break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

void ast_print(node * ast) {

}
