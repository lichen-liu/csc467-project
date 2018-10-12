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
//#include "ast.h"
//#include "symbol.h"
//#include "semantic.h"
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
  int as_int;
  float as_float;
  
  char as_id[MAX_IDENTIFIER + 1];
  char as_func[MAX_IDENTIFIER + 1];
}
// TODO:Replace myToken with your tokens, you can use these tokens in flex
// String keywords
%token           IF_SYM ELSE_SYM
%token           WHILE_SYM
%token           TRUE_SYM FALSE_SYM
%token           CONST_SYM

// Predefined
%token           BOOL_T BVEC2_T BVEC3_T BVEC4_T
%token           INT_T IVEC2_T IVEC3_T IVEC4_T
%token           FLOAT_T VEC2_T VEC3_T VEC4_T
%token           FUNC

// Symbols
%token           NOT AND OR
%token           PLUS MINUS TIMES SLASH EXP
%token           EQL NEQ LSS LEQ GTR GEQ
%token           LPAREN RPAREN
%token           LBRACE RBRACE
%token           LBRACKET RBRACKET
%token           ASSGNMT
%token           SEMICOLON
%token           COMMA

// Literals
%token           INT_C
%token           FLOAT_C

// Identifier
%token           ID

%start    program

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
  :   scope
  ;
scope
  :   LBRACE declarations statements RBRACE
  ;
declarations
  :  declarations declaration  
  |  /* epsilon */    
  ;
statements
  :  statements statement  
  |  /* epsilon */    
  ;
declaration
  :  type ID SEMICOLON
  |  type ID ASSGNMT expression SEMICOLON
  |  CONST_SYM type ID ASSGNMT expression SEMICOLON
  |  /* epsilon */
  ;
statement
  :  variable ASSGNMT expression SEMICOLON
  |  IF_SYM LPAREN expression RPAREN statement else_statement
  |  WHILE_SYM LPAREN expression RPAREN statement
  |  scope
  |  SEMICOLON
  ;
else_statement
  :  ELSE_SYM statement  
  |  /* epsilon */    
  ;
type
  :  INT_T | IVEC2_T | IVEC3_T | IVEC4_T
  |  BOOL_T | BVEC2_T | BVEC3_T  | BVEC4_T
  |  FLOAT_T | VEC2_T | VEC3_T | VEC4_T
  ;
expression
  :  constructor
  |  function
  |  INT_C
  |  FLOAT_C
  |  TRUE_SYM | FALSE_SYM
  |  variable
  |  unary_op expression
  |  expression binary_op expression
  |  LPAREN expression RPAREN
  ;
variable
  :  ID
  |  ID LBRACKET INT_C RBRACKET
  ;
unary_op
  :  NOT
  |  MINUS
  ;
binary_op
  :  AND
  |  OR
  |  EQL    
  |  NEQ
  |  LSS
  |  LEQ
  |  GTR
  |  GEQ
  |  PLUS
  |  MINUS
  |  TIMES
  |  SLASH
  |  EXP
  ;
constructor
  :  type LPAREN arguments RPAREN
  ;
function
  :  function_name LPAREN arguments_opt RPAREN
  ;
function_name
  :  FUNC
  ;
arguments_opt
  :  arguments
  |  /* epsilon */
  ;
arguments
  :  arguments COMMA expression
  |  expression
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

