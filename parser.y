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

// Token declarations
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


// Precedence declarations
// lower declaration, higher precedence
// precedence 7
%left            OR

// precedence 6
%left            AND

// precedence 5
%nonassoc        EQL NEQ LSS LEQ GTR GEQ

// precedence 4
%left            PLUS MINUS

// precedence 3
%left            TIMES SLASH

// precedence 2
%right           EXP

// precedence 1
%left            UNARY_PREC

// precedence 0
%left            VECT_FUNC_CONS_PREC


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
  :   scope                                                       { yTRACE("program -> scope");                                                       }
  ;
scope
  :   LBRACE declarations statements RBRACE                       { yTRACE("scope -> { declarations statements }");                                   }
  ;
declarations
  :  declarations declaration                                     { yTRACE("declarations -> declarations declaration");                               }
  |  epsilon                                                      { yTRACE("declarations -> epsilon");                                                }
  ;
statements
  :  statements statement                                         { yTRACE("statements -> statements statement");                                     }
  |  epsilon                                                      { yTRACE("statements -> epsilon");                                                  }
  ;
declaration
  :  type ID SEMICOLON                                            { yTRACE("declaration -> type ID ;");                                               }
  |  type ID ASSGNMT expression SEMICOLON                         { yTRACE("declaration -> type ID = expression ;");                                  }
  |  CONST_SYM type ID ASSGNMT expression SEMICOLON               { yTRACE("declaration -> const type ID = expression ;");                            }
  |  epsilon                                                      { yTRACE("declaration -> epsilon");                                                 }
  ;
statement
  :  variable ASSGNMT expression SEMICOLON                        { yTRACE("statement -> variable = expression ;");                                   }
  |  IF_SYM LPAREN expression RPAREN statement else_statement     { yTRACE("statement -> if ( expression ) statement else_statement");                }
  |  WHILE_SYM LPAREN expression RPAREN statement                 { yTRACE("statement -> while ( expression ) statement");                            }
  |  scope                                                        { yTRACE("statement -> scope");                                                     }
  |  SEMICOLON                                                    { yTRACE("statement -> ;");                                                         }
  ;
else_statement
  :  ELSE_SYM statement                                           { yTRACE("else_statement -> else statement");                                       }
  |  epsilon                                                      { yTRACE("else_statement -> epsilon");                                              }
  ;
type
  :  INT_T | IVEC2_T | IVEC3_T | IVEC4_T                          { yTRACE("type -> int | ivec2 | ivec3 | ivec4");                                    }
  |  BOOL_T | BVEC2_T | BVEC3_T  | BVEC4_T                        { yTRACE("type -> bool | bvec2 | bvec3 | bvec4");                                   }
  |  FLOAT_T | VEC2_T | VEC3_T | VEC4_T                           { yTRACE("type -> float | vec2 | vec3 | vec4");                                     }
  ;
expression
  :  constructor                                                  { yTRACE("expression -> constructor");                                              }
  |  function                                                     { yTRACE("expression -> function");                                                 }
  |  INT_C                                                        { yTRACE("expression -> integer_literal");                                          }
  |  FLOAT_C                                                      { yTRACE("expression -> float_literal");                                            }
  |  TRUE_SYM | FALSE_SYM                                         { yTRACE("expression -> true | false");                                             }
  |  variable                                                     { yTRACE("expression -> variable");                                                 }
  |  unary_op expression                                          { yTRACE("expression -> unary_op expression");                                      }
  |  expression binary_op expression                              { yTRACE("expression -> expression binary_op expression");                          }
  |  LPAREN expression RPAREN                                     { yTRACE("expression -> (expression)");                                             }
  ;
variable
  :  ID                                                           { yTRACE("variable -> identifier");                                                 }
  |  ID LBRACKET INT_C RBRACKET      %prec VECT_FUNC_CONS_PREC    { yTRACE("variable -> identifier[integer_literal]");                                }
  ;
unary_op
  :  NOT                             %prec UNARY_PREC             { yTRACE("unary_op -> !");                                                          }
  |  MINUS                           %prec UNARY_PREC             { yTRACE("unary_op -> -");                                                          }
  ;
binary_op
  :  AND                                                          { yTRACE("binary_op -> &&");                                                        }
  |  OR                                                           { yTRACE("binary_op -> ||");                                                        }
  |  EQL                                                          { yTRACE("binary_op -> ==");                                                        }
  |  NEQ                                                          { yTRACE("binary_op -> !=");                                                        }
  |  LSS                                                          { yTRACE("binary_op -> <");                                                         }
  |  LEQ                                                          { yTRACE("binary_op -> <=");                                                        }
  |  GTR                                                          { yTRACE("binary_op -> >");                                                         }
  |  GEQ                                                          { yTRACE("binary_op -> >=");                                                        }
  |  PLUS                                                         { yTRACE("binary_op -> +");                                                         }
  |  MINUS                                                        { yTRACE("binary_op -> -");                                                         }
  |  TIMES                                                        { yTRACE("binary_op -> *");                                                         }
  |  SLASH                                                        { yTRACE("binary_op -> /");                                                         }
  |  EXP                                                          { yTRACE("binary_op -> ^");                                                         }
  ;
constructor
  :  type LPAREN arguments RPAREN    %prec VECT_FUNC_CONS_PREC    { yTRACE("constructor -> type ( arguments )");                                      }
  ;
function
  :  function_name LPAREN arguments_opt RPAREN    %prec VECT_FUNC_CONS_PREC   { yTRACE("function -> function_name ( arguments_opt )");                }
  ;
function_name
  :  FUNC                                                         { yTRACE("function_name -> lit | dp3 | rsq");                                       }
  ;
arguments_opt
  :  arguments                                                    { yTRACE("arguments_opt -> arguments");                                             }
  |  epsilon                                                      { yTRACE("arguments_opt -> epsilon");                                               }
  ;
arguments
  :  arguments COMMA expression                                   { yTRACE("arguments -> arguments , expression");                                    }
  |  expression                                                   { yTRACE("arguments -> expression");                                                }
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

