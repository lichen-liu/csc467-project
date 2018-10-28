#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

//////////////////////////////////////////////////////////////////
//
// A Modern Object-Oriented Approach for AST
//
//////////////////////////////////////////////////////////////////
class Node;


typedef struct Node node;
extern node *ast;


typedef enum {
    UNKNOWN               = 0,

    SCOPE_NODE               ,

    EXPRESSION_NODE          ,
    EXPRESSIONS_NODE         ,
    UNARY_EXPRESION_NODE     ,
    BINARY_EXPRESSION_NODE   ,
    INT_C_NODE               ,
    FLOAT_C_NODE             ,
    BOOL_C_NODE              ,
    VAR_NODE                 ,
    ID_NODE                  ,
    INDEXING_NODE            ,
    FUNCTION_NODE            ,
    CONSTRUCTOR_NODE         ,

    STATEMENT_NODE           ,
    IF_STATEMENT_NODE        ,
    WHILE_STATEMENT_NODE     ,
    ASSIGNMENT_NODE          ,
    NESTED_SCOPE_NODE        ,
    STALL_STATEMENT_NODE     ,
    STATEMENTS_NODE          ,

    DECLARATION_NODE         ,
    DECLARATIONS_NODE
} node_kind;

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);
int semantic_check(node * ast);

#endif /* AST_H_ */
