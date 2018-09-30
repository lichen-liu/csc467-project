%{
/**********************************************************************
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include "string.h"

#include <stdarg.h>

#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }

#ifdef TEST_SCANNER
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); }
#else
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }
#endif


int yyline = 1;

// #define SCANNER_DEBUG
/* Debug output when debug flag is defined */
void dbprtf(const char* fmt, ...);

/* TRUE if valid */
int CheckInt(int *int_value);
int CheckFloat(float *float_value);
int CheckID(char *id_value);

/* Return nothing */
void ToFunc(char *func_value);

%}
%option noyywrap

WS                              [ \t]

DIGIT                           [0-9]
SIGN                            [\+\-]

ID                              [A-Za-z_][A-Za-z0-9_]*

INT_LIT                         {DIGIT}+
LR_FLOAT_LIT                    {DIGIT}+\.{DIGIT}+
L_FLOAT_LIT                     {DIGIT}+\.
R_FLOAT_LIT                     \.{DIGIT}+
EXP_LIT                         [eE]{SIGN}?{DIGIT}+


%x                              IN_COMMENT

%%
"/*"                            { BEGIN(IN_COMMENT);                                                                                            }
<IN_COMMENT>"*/"                { BEGIN(INITIAL);                                                                                               }

"if"                            { yTRACE(IF_SYM); return IF_SYM;                                                                                }
"else"                          { yTRACE(ELSE_SYM); return ELSE_SYM;                                                                            }
"while"                         { yTRACE(WHILE_SYM); return WHILE_SYM;                                                                          }
"true"                          { yTRACE(TRUE_SYM); return TRUE_SYM;                                                                            }
"false"                         { yTRACE(FALSE_SYM); return FALSE_SYM;                                                                          }
"const"                         { yTRACE(CONST_SYM); return CONST_SYM;                                                                          }

"bool"                          { yTRACE(BOOL_T); return BOOL_T;                                                                                }
"bvec2"                         { yTRACE(BVEC2_T); return BVEC2_T;                                                                              }
"bvec3"                         { yTRACE(BVEC3_T); return BVEC3_T;                                                                              }
"bvec4"                         { yTRACE(BVEC4_T); return BVEC4_T;                                                                              }
"int"                           { yTRACE(INT_T); return INT_T;                                                                                  }
"ivec2"                         { yTRACE(IVEC2_T); return IVEC2_T;                                                                              }
"ivec3"                         { yTRACE(IVEC3_T); return IVEC3_T;                                                                              }
"ivec4"                         { yTRACE(IVEC4_T); return IVEC4_T;                                                                              }
"float"                         { yTRACE(FLOAT_T); return FLOAT_T;                                                                              }
"vec2"                          { yTRACE(VEC2_T); return VEC2_T;                                                                                }
"vec3"                          { yTRACE(VEC3_T); return VEC3_T;                                                                                }
"vec4"                          { yTRACE(VEC4_T); return VEC4_T;                                                                                }

"lit"|"dp3"|"rsq"               { ToFunc(yylval.as_func); yTRACE(FUNC); dbprtf("%s\n", yylval.as_func); return FUNC;                            }

"=="                            { yTRACE(EQL); return EQL;                                                                                      }
"!="                            { yTRACE(NEQ); return NEQ;                                                                                      }
"<="                            { yTRACE(LEQ); return LEQ;                                                                                      }
"<"                             { yTRACE(LSS); return LSS;                                                                                      }
">="                            { yTRACE(GEQ); return GEQ;                                                                                      }
">"                             { yTRACE(GTR); return GTR;                                                                                      }
"&&"                            { yTRACE(AND); return AND;                                                                                      }
"||"                            { yTRACE(OR); return OR;                                                                                        }
"!"                             { yTRACE(NOT); return NOT;                                                                                      }
"+"                             { yTRACE(PLUS); return PLUS;                                                                                    }
"-"                             { yTRACE(MINUS); return MINUS;                                                                                  }
"*"                             { yTRACE(TIMES); return TIMES;                                                                                  }
"/"                             { yTRACE(SLASH); return SLASH;                                                                                  }
"^"                             { yTRACE(EXP); return EXP;                                                                                      }
"("                             { yTRACE(LPAREN); return LPAREN;                                                                                }
")"                             { yTRACE(RPAREN); return RPAREN;                                                                                }
"{"                             { yTRACE(LBRACE); return LBRACE;                                                                                }
"}"                             { yTRACE(RBRACE); return RBRACE;                                                                                }
"["                             { yTRACE(LBRACKET); return LBRACKET;                                                                            }
"]"                             { yTRACE(RBRACKET); return RBRACKET;                                                                            }
"="                             { yTRACE(ASSGNMT); return ASSGNMT;                                                                              }
";"                             { yTRACE(SEMICOLON); return SEMICOLON;                                                                          }
","                             { yTRACE(COMMA); return COMMA;                                                                                  }

{ID}                            { if(CheckID(yylval.as_id)){ yTRACE(ID); dbprtf("%s\n", yylval.as_id); return ID;                             } }

{INT_LIT}{EXP_LIT}              { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C;         } }
{LR_FLOAT_LIT}{EXP_LIT}         { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C;         } }
{L_FLOAT_LIT}{EXP_LIT}          { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C;         } }
{R_FLOAT_LIT}{EXP_LIT}          { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C;         } }
{INT_LIT}                       { if(CheckInt(&yylval.as_int)){ yTRACE(INT_C); dbprtf("%d\n", yylval.as_int); return INT_C;                   } }
{LR_FLOAT_LIT}                  { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%f\n", yylval.as_float); return FLOAT_C;         } }
{L_FLOAT_LIT}                   { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%f\n", yylval.as_float); return FLOAT_C;         } }
{R_FLOAT_LIT}                   { if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%f\n", yylval.as_float); return FLOAT_C;         } }

{INT_LIT}{EXP_LIT}{ID}          { yERROR("Float literal should not be followed by identifier");                                                 }
{LR_FLOAT_LIT}{EXP_LIT}{ID}     { yERROR("Float literal should not be followed by identifier");                                                 }
{L_FLOAT_LIT}{EXP_LIT}{ID}      { yERROR("Float literal should not be followed by identifier");                                                 }
{R_FLOAT_LIT}{EXP_LIT}{ID}      { yERROR("Float literal should not be followed by identifier");                                                 }
{INT_LIT}{ID}                   { yERROR("Integer literal should not be followed by identifier");                                               }
{LR_FLOAT_LIT}{ID}              { yERROR("Float literal should not be followed by identifier");                                                 }
{L_FLOAT_LIT}{ID}               { yERROR("Float literal should not be followed by identifier");                                                 }
{R_FLOAT_LIT}{ID}               { yERROR("Float literal should not be followed by identifier");                                                 }

{WS}                            /* skip whitespace */
<*>\n                           { yyline++;                                                                                                     }

<IN_COMMENT>.                   { dbprtf("COMMENT SKIPPED <LINE %d>:%s\n", yyline, yytext);                                                     }
.                               { dbprtf("=> LINE %d : %s\n", yyline, yytext); yERROR("Unknown token");                                         }

<IN_COMMENT><<EOF>>             { BEGIN(INITIAL); yERROR("Comment block is not closed by */ until EOF"); errorOccurred = TRUE; yyterminate();   }
%%

void dbprtf(const char* fmt, ...) {
#ifdef SCANNER_DEBUG
    va_list args;
    va_start(args, fmt);

    // Forward to vfprintf()
    if(traceScanner) {
        vfprintf(traceFile, fmt, args);
    }
    

    va_end(args);
#endif
}

int CheckInt(int *int_value) {
    char *pend;
    long int integer_value = strtol(yytext, &pend, 10);

    /* check int start with 0 */
    if(strlen(yytext) >= 2 && yytext[0] == '0') {
        yERROR("Integer literal cannot start with 0");
        *int_value = 0;
        return FALSE;
    }

    /* bound-check */
    if(integer_value > MAX_INTEGER) {
        yERROR("Integer literal is not within the legal range");
        *int_value = MAX_INTEGER;
        return FALSE;
    }
    
    *int_value = integer_value; // cast from long to int
    return TRUE;
}

int CheckFloat(float *float_value) {
    char *pend;
    double double_value = strtod(yytext, &pend);

    /* check float start with 0 */
    if(strlen(yytext) >= 2 && yytext[0] == '0' && !(yytext[1] == '.' || yytext[1] == 'e' || yytext[1] == 'E')) {
        yERROR("Float literal cannot start with 0");
        *float_value = 0.0;
        return FALSE;
    }

    /* bound-check */
    if(double_value != 0.0) {
        if(double_value < MIN_FLOAT) {
            yERROR("Float literal is not within the legal range");
            *float_value = MIN_FLOAT;
            return FALSE;
        } else if(double_value > MAX_FLOAT) {
            yERROR("Float literal is not within the legal range");
            *float_value = MAX_FLOAT;
            return FALSE;
        }
    }

    *float_value = double_value; // cast from double to float
    return TRUE;
}

int CheckID(char *id_value) {
    /* identifier length check */
    if(strlen(yytext) > MAX_IDENTIFIER) {
        /* identifier <= MAX_IDENTIFIER 
         * identifier + '\0' <= MAX_IDENTIFIER + 1 */
        yERROR("Identifier has a illegal length");
        strcpy(id_value, "");
        return FALSE;
    }

    strcpy(id_value, yytext);
    return TRUE;
}

void ToFunc(char *func_value) {
    /* predefined functions have known length */
    strcpy(func_value, yytext);
}