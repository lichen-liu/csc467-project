#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union.
//

// Forward declarations
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum {
    UNKNOWN               = 0,

    SCOPE_NODE               ,

    EXPRESSION_NODE          ,
    UNARY_EXPRESION_NODE     ,
    BINARY_EXPRESSION_NODE   ,
    INT_NODE                 , 
    FLOAT_NODE               ,
    IDENT_NODE               ,
    VAR_NODE                 ,
    FUNCTION_NODE            ,
    CONSTRUCTOR_NODE         ,

    STATEMENT_NODE           ,
    IF_STATEMENT_NODE        ,
    WHILE_STATEMENT_NODE     ,
    ASSIGNMENT_NODE          ,
    NESTED_SCOPE_NODE        ,
    STATEMENTS_NODE          ,

    DECLARATION_NODE         ,
    DECLARATIONS_NODE
} node_kind;

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);
int semantic_check(node * ast);

struct node_ {
    // an example of tagging each node with a type
    node_kind kind;

    union {
    struct {
        // declarations?
        // statements?
    } scope;

    struct {
        int op;
        node *right;
    } unary_expr;

    struct {
        int op;
        node *left;
        node *right;
    } binary_expr;

    // TODO: add more type of nodes
    };
};

//////////////////////////////////////////////////////////////////
//
// A Modern Object-Oriented Approach for AST
//
//////////////////////////////////////////////////////////////////

/*
 * Forward Declaration for Vistors
 */
class Visitor;
class PrintVisitor;

/*
 * Base class for AST Nodes
 */
class Node {
    public:
        virtual void visit(Visitor &vistor) = 0;
        virtual void print(PrintVisitor &printVisitor) = 0;
};

#endif /* AST_H_ */
