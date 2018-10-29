%{
/***********************************************************************
 *   Interface to the parser module for CSC467 course project.
 * 
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source language program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>
#include "common.h"
#include "ast.h"
#include "symbol.h"
// #include "semantic.h"
#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */

%}

/***********************************************************************
 *  Yacc/Bison declarations.
 *  Phase 2:
 *    1. Add precedence declarations for operators (after %start declaration)
 *    2. If necessary, add %type declarations for some nonterminals
 *  Phase 3:
 *    1. Add fields to the union below to facilitate the construction of the
 *       AST (the two existing fields allow the lexical analyzer to pass back
 *       semantic info, so they shouldn't be touched).
 *    2. Add <type> modifiers to appropriate %token declarations (using the
 *       fields of the union) so that semantic information can by passed back
 *       by the scanner.
 *    3. Make the %type declarations for the language non-terminals, utilizing
 *       the fields of the union as well.
 ***********************************************************************/

%{
#define YYDEBUG 1
%}


// TODO:Modify me to add more data types
// Can access me from flex useing yyval
%union {
  /* Lexer */
  int as_int;
  float as_float;
  
  char as_id[MAX_IDENTIFIER + 1];
  char as_func[MAX_IDENTIFIER + 1];

  /* Parser */
  int op_type;
  int data_type;
  node *ast_node;
}


/* Token declarations */
// String keywords
%token              IF_SYM ELSE_SYM
%token              WHILE_SYM
%token              TRUE_SYM FALSE_SYM
%token              CONST_SYM

// Predefined
%token              BOOL_T BVEC2_T BVEC3_T BVEC4_T
%token              INT_T IVEC2_T IVEC3_T IVEC4_T
%token              FLOAT_T VEC2_T VEC3_T VEC4_T
%token <as_func>    FUNC

// Symbols
%token              NOT AND OR
%token              PLUS MINUS TIMES SLASH EXP
%token              EQL NEQ LSS LEQ GTR GEQ
%token              LPAREN RPAREN
%token              LBRACE RBRACE
%token              LBRACKET RBRACKET
%token              ASSGNMT
%token              SEMICOLON
%token              COMMA

// Literals
%token <as_int>     INT_C
%token <as_float>   FLOAT_C

// Identifier
%token <as_id>      ID


/* Non-terminal declarations */
%type <ast_node>    scope
%type <ast_node>    declarations
%type <ast_node>    statements
%type <ast_node>    declaration
%type <ast_node>    statement
%type <ast_node>    else_statement
%type <data_type>   type
%type <ast_node>    expression
%type <ast_node>    variable
%type <op_type>     unary_op
%type <ast_node>    constructor
%type <ast_node>    function
%type <as_func>     function_name
%type <ast_node>    arguments_opt
%type <ast_node>    arguments

%start              program


/* Precedence declarations */
// lower declaration, higher precedence
// precedence 7
%left               OR

// precedence 6
%left               AND

// precedence 5
%nonassoc           EQL NEQ LSS LEQ GTR GEQ

// precedence 4
%left               PLUS MINUS

// precedence 3
%left               TIMES SLASH

// precedence 2
%right              EXP

// precedence 1
%left               UNARY_PREC

// precedence 0
%left               V_F_C_PREC


%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler
 *  Phase 3:
 *    1. Add code to rules for construction of AST.
 ***********************************************************************/
program
  :  scope                                                          { yTRACE("program -> scope");                                         ast = $1;}
  ;
scope
  :  LBRACE declarations statements RBRACE                          { yTRACE("scope -> { declarations statements }");                     $$ = ast_allocate(SCOPE_NODE, $2, $3);}
  ;
declarations
  :  declarations declaration                                       { yTRACE("declarations -> declarations declaration");                 $$ = ast_allocate(DECLARATIONS_NODE, $1, $2);}
  |  epsilon                                                        { yTRACE("declarations -> epsilon");                                  $$ = ast_allocate(DECLARATIONS_NODE, nullptr, nullptr);}
  ;
statements
  :  statements statement                                           { yTRACE("statements -> statements statement");                       $$ = ast_allocate(STATEMENTS_NODE, $1, $2);}
  |  epsilon                                                        { yTRACE("statements -> epsilon");                                    $$ = ast_allocate(STATEMENTS_NODE, nullptr, nullptr);}
  ;
declaration
  :  type ID SEMICOLON                                              { yTRACE("declaration -> type ID ;");                                 $$ = ast_allocate(DECLARATION_NODE, $2, 0, $1, nullptr);}
  |  type ID ASSGNMT expression SEMICOLON                           { yTRACE("declaration -> type ID = expression ;");                    $$ = ast_allocate(DECLARATION_NODE, $2, 0, $1, $4);}
  |  CONST_SYM type ID ASSGNMT expression SEMICOLON                 { yTRACE("declaration -> const type ID = expression ;");              $$ = ast_allocate(DECLARATION_NODE, $3, 1, $2, $5);}
  ;
statement
  :  variable ASSGNMT expression SEMICOLON                          { yTRACE("statement -> variable = expression ;");                     $$ = ast_allocate(ASSIGNMENT_NODE, $1, $3);}
  |  IF_SYM LPAREN expression RPAREN statement else_statement       { yTRACE("statement -> if ( expression ) statement else_statement");  $$ = ast_allocate(IF_STATEMENT_NODE, $3, $5, $6);}
  |  WHILE_SYM LPAREN expression RPAREN statement                   { yTRACE("statement -> while ( expression ) statement");              $$ = ast_allocate(WHILE_STATEMENT_NODE, $3, $5);}
  |  scope                                                          { yTRACE("statement -> scope");                                       $$ = ast_allocate(NESTED_SCOPE_NODE, $1); }
  |  SEMICOLON                                                      { yTRACE("statement -> ;");                                           $$ = ast_allocate(STALL_STATEMENT_NODE);}
  ;
else_statement
  :  ELSE_SYM statement                                             { yTRACE("else_statement -> else statement");                         $$ = $2;}
  |  epsilon                                                        { yTRACE("else_statement -> epsilon");                                $$ = nullptr;}
  ;
type
  :  INT_T                                                          { yTRACE("type -> int");                                              $$ = INT_T;}
  |  IVEC2_T                                                        { yTRACE("type -> ivec2");                                            $$ = IVEC2_T;}
  |  IVEC3_T                                                        { yTRACE("type -> ivec3");                                            $$ = IVEC3_T;}
  |  IVEC4_T                                                        { yTRACE("type -> ivec4");                                            $$ = IVEC4_T;}
  |  BOOL_T                                                         { yTRACE("type -> bool");                                             $$ = BOOL_T;}
  |  BVEC2_T                                                        { yTRACE("type -> bvec2");                                            $$ = BVEC2_T;}
  |  BVEC3_T                                                        { yTRACE("type -> bvec3");                                            $$ = BVEC3_T;}
  |  BVEC4_T                                                        { yTRACE("type -> bvec4");                                            $$ = BVEC4_T;}
  |  FLOAT_T                                                        { yTRACE("type -> float");                                            $$ = FLOAT_T;}
  |  VEC2_T                                                         { yTRACE("type -> vec2");                                             $$ = VEC2_T;}
  |  VEC3_T                                                         { yTRACE("type -> vec3");                                             $$ = VEC3_T;}
  |  VEC4_T                                                         { yTRACE("type -> vec4");                                             $$ = VEC4_T;}
  ;
expression
  :  constructor                                                    { yTRACE("expression -> constructor");                                $$ = $1;}
  |  function                                                       { yTRACE("expression -> function");                                   $$ = $1;}
  |  INT_C                                                          { yTRACE("expression -> integer_literal");                            $$ = ast_allocate(INT_C_NODE, $1);}
  |  FLOAT_C                                                        { yTRACE("expression -> float_literal");                              $$ = ast_allocate(FLOAT_C_NODE, $1);}
  |  TRUE_SYM                                                       { yTRACE("expression -> true");                                       $$ = ast_allocate(BOOL_C_NODE, 1);}
  |  FALSE_SYM                                                      { yTRACE("expression -> false");                                      $$ = ast_allocate(BOOL_C_NODE, 0);}
  |  variable                                                       { yTRACE("expression -> variable");                                   $$ = $1;}
  |  unary_op expression                          %prec UNARY_PREC  { yTRACE("expression -> unary_op expression");                        $$ = ast_allocate(UNARY_EXPRESION_NODE, $1, $2);}
  |  expression AND expression                                      { yTRACE("expression -> expression && expression");                   $$ = ast_allocate(BINARY_EXPRESSION_NODE, AND, $1, $3);}
  |  expression OR expression                                       { yTRACE("expression -> expression || expression");                   $$ = ast_allocate(BINARY_EXPRESSION_NODE, OR, $1, $3);}
  |  expression EQL expression                                      { yTRACE("expression -> expression == expression");                   $$ = ast_allocate(BINARY_EXPRESSION_NODE, EQL, $1, $3);}
  |  expression NEQ expression                                      { yTRACE("expression -> expression != expression");                   $$ = ast_allocate(BINARY_EXPRESSION_NODE, NEQ, $1, $3);}
  |  expression LSS expression                                      { yTRACE("expression -> expression < expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, LSS, $1, $3);}
  |  expression LEQ expression                                      { yTRACE("expression -> expression <= expression");                   $$ = ast_allocate(BINARY_EXPRESSION_NODE, LEQ, $1, $3);}
  |  expression GTR expression                                      { yTRACE("expression -> expression > expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, GTR, $1, $3);}
  |  expression GEQ expression                                      { yTRACE("expression -> expression >= expression");                   $$ = ast_allocate(BINARY_EXPRESSION_NODE, GEQ, $1, $3);}
  |  expression PLUS expression                                     { yTRACE("expression -> expression + expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, PLUS, $1, $3);}
  |  expression MINUS expression                                    { yTRACE("expression -> expression - expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, MINUS, $1, $3);}
  |  expression TIMES expression                                    { yTRACE("expression -> expression * expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, TIMES, $1, $3);}
  |  expression SLASH expression                                    { yTRACE("expression -> expression / expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, SLASH, $1, $3);}
  |  expression EXP expression                                      { yTRACE("expression -> expression ^ expression");                    $$ = ast_allocate(BINARY_EXPRESSION_NODE, EXP, $1, $3);}
  |  LPAREN expression RPAREN                                       { yTRACE("expression -> (expression)");                               $$ = $2;}
  ;
variable
  :  ID                                                             { yTRACE("variable -> identifier");                                   $$ = ast_allocate(ID_NODE, $1);}
  |  ID LBRACKET INT_C RBRACKET                   %prec V_F_C_PREC  { yTRACE("variable -> identifier[integer_literal]");                  $$ = ast_allocate(INDEXING_NODE, $1, ast_allocate(INT_C_NODE, $3));}
  ;
unary_op
  :  NOT                                                            { yTRACE("unary_op -> !");                                            $$ = NOT;}
  |  MINUS                                                          { yTRACE("unary_op -> -");                                            $$ = MINUS;}
  ;
constructor
  :  type LPAREN arguments RPAREN                 %prec V_F_C_PREC  { yTRACE("constructor -> type ( arguments )");                        $$ = ast_allocate(CONSTRUCTOR_NODE, $1, $3);}
  ;
function
  :  function_name LPAREN arguments_opt RPAREN    %prec V_F_C_PREC  { yTRACE("function -> function_name ( arguments_opt )");              $$ = ast_allocate(FUNCTION_NODE, $1, $3);}
  ;
function_name
  :  FUNC                                                           { yTRACE("function_name -> lit | dp3 | rsq");                         strcpy($$, $1);}
  ;
arguments_opt
  :  arguments                                                      { yTRACE("arguments_opt -> arguments");                               $$ = $1;}
  |  epsilon                                                        { yTRACE("arguments_opt -> epsilon");                                 $$ = ast_allocate(EXPRESSIONS_NODE, nullptr, nullptr);}
  ;
arguments
  :  arguments COMMA expression                                     { yTRACE("arguments -> arguments , expression");                      $$ = ast_allocate(EXPRESSIONS_NODE, $1, $3);}
  |  expression                                                     { yTRACE("arguments -> expression");                                  $$ = ast_allocate(EXPRESSIONS_NODE, nullptr, $1);}
  ;
epsilon
  :
  ;

%%

/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(const char* s) {
  if (errorOccurred)
    return;    /* Error has already been reported by scanner */
  else
    errorOccurred = 1;
        
  fprintf(errorFile, "\nPARSER ERROR, LINE %d",yyline);
  if (strcmp(s, "parse error")) {
    if (strncmp(s, "parse error, ", 13))
      fprintf(errorFile, ": %s\n", s);
    else
      fprintf(errorFile, ": %s\n", s+13);
  } else
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
}

