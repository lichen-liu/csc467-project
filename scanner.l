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

int inCommentBlock = FALSE;
int CheckCommentHead(void);
int CheckCommentTail(void);
/* TRUE if in multi-line comment block */
int CheckCommentBody(void);

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


%s                              IN_MULTILINE_COMMENT

%%
"/*".*"*/"                      { if(!CheckCommentBody()){ dbprtf("SINGLELINE COMMENT, LINE %d : %s\n", yyline, yytext);                                         } }
"/*".*                          { CheckCommentHead();                                                                                                              }
.*"*/"                          { CheckCommentTail();                                                                                                              }

"if"                            { if(!CheckCommentBody()){ yTRACE(IF_SYM); return IF_SYM;                                                                        } }
"else"                          { if(!CheckCommentBody()){ yTRACE(ELSE_SYM); return ELSE_SYM;                                                                    } }
"while"                         { if(!CheckCommentBody()){ yTRACE(WHILE_SYM); return WHILE_SYM;                                                                  } }
"true"                          { if(!CheckCommentBody()){ yTRACE(TRUE_SYM); return TRUE_SYM;                                                                    } }
"false"                         { if(!CheckCommentBody()){ yTRACE(FALSE_SYM); return FALSE_SYM;                                                                  } }
"const"                         { if(!CheckCommentBody()){ yTRACE(CONST_SYM); return CONST_SYM;                                                                  } }

"bool"                          { if(!CheckCommentBody()){ yTRACE(BOOL_T); return BOOL_T;                                                                        } }
"bvec2"                         { if(!CheckCommentBody()){ yTRACE(BVEC2_T); return BVEC2_T;                                                                      } }
"bvec3"                         { if(!CheckCommentBody()){ yTRACE(BVEC3_T); return BVEC3_T;                                                                      } }
"bvec4"                         { if(!CheckCommentBody()){ yTRACE(BVEC4_T); return BVEC4_T;                                                                      } }
"int"                           { if(!CheckCommentBody()){ yTRACE(INT_T); return INT_T;                                                                          } }
"ivec2"                         { if(!CheckCommentBody()){ yTRACE(IVEC2_T); return IVEC2_T;                                                                      } }
"ivec3"                         { if(!CheckCommentBody()){ yTRACE(IVEC3_T); return IVEC3_T;                                                                      } }
"ivec4"                         { if(!CheckCommentBody()){ yTRACE(IVEC4_T); return IVEC4_T;                                                                      } }
"float"                         { if(!CheckCommentBody()){ yTRACE(FLOAT_T); return FLOAT_T;                                                                      } }
"vec2"                          { if(!CheckCommentBody()){ yTRACE(VEC2_T); return VEC2_T;                                                                        } }
"vec3"                          { if(!CheckCommentBody()){ yTRACE(VEC3_T); return VEC3_T;                                                                        } }
"vec4"                          { if(!CheckCommentBody()){ yTRACE(VEC4_T); return VEC4_T;                                                                        } }

"lit"|"dp3"|"rsq"               { if(!CheckCommentBody()){ ToFunc(yylval.as_func); yTRACE(FUNC); dbprtf("%s\n", yylval.as_func); return FUNC;                    } }

"=="                            { if(!CheckCommentBody()){ yTRACE(EQL); return EQL;                                                                              } }
"!="                            { if(!CheckCommentBody()){ yTRACE(NEQ); return NEQ;                                                                              } }
"<="                            { if(!CheckCommentBody()){ yTRACE(LEQ); return LEQ;                                                                              } }
"<"                             { if(!CheckCommentBody()){ yTRACE(LSS); return LSS;                                                                              } }
">="                            { if(!CheckCommentBody()){ yTRACE(GEQ); return GEQ;                                                                              } }
">"                             { if(!CheckCommentBody()){ yTRACE(GTR); return GTR;                                                                              } }
"&&"                            { if(!CheckCommentBody()){ yTRACE(AND); return AND;                                                                              } }
"||"                            { if(!CheckCommentBody()){ yTRACE(OR); return OR;                                                                                } }
"!"                             { if(!CheckCommentBody()){ yTRACE(NOT); return NOT;                                                                              } }
"+"                             { if(!CheckCommentBody()){ yTRACE(PLUS); return PLUS;                                                                            } }
"-"                             { if(!CheckCommentBody()){ yTRACE(MINUS); return MINUS;                                                                          } }
"*"                             { if(!CheckCommentBody()){ yTRACE(TIMES); return TIMES;                                                                          } }
"/"                             { if(!CheckCommentBody()){ yTRACE(SLASH); return SLASH;                                                                          } }
"^"                             { if(!CheckCommentBody()){ yTRACE(EXP); return EXP;                                                                              } }
"("                             { if(!CheckCommentBody()){ yTRACE(LPAREN); return LPAREN;                                                                        } }
")"                             { if(!CheckCommentBody()){ yTRACE(RPAREN); return RPAREN;                                                                        } }
"{"                             { if(!CheckCommentBody()){ yTRACE(LBRACE); return LBRACE;                                                                        } }
"}"                             { if(!CheckCommentBody()){ yTRACE(RBRACE); return RBRACE;                                                                        } }
"["                             { if(!CheckCommentBody()){ yTRACE(LBRACKET); return LBRACKET;                                                                    } }
"]"                             { if(!CheckCommentBody()){ yTRACE(RBRACKET); return RBRACKET;                                                                    } }
"="                             { if(!CheckCommentBody()){ yTRACE(ASSGNMT); return ASSGNMT;                                                                      } }
";"                             { if(!CheckCommentBody()){ yTRACE(SEMICOLON); return SEMICOLON;                                                                  } }
","                             { if(!CheckCommentBody()){ yTRACE(COMMA); return COMMA;                                                                          } }

{ID}                            { if(!CheckCommentBody()){ if(CheckID(yylval.as_id)){ yTRACE(ID); dbprtf("%s\n", yylval.as_id); return ID;                     } } }

{INT_LIT}{EXP_LIT}              { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C; } } }
{LR_FLOAT_LIT}{EXP_LIT}         { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C; } } }
{L_FLOAT_LIT}{EXP_LIT}          { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C; } } }
{R_FLOAT_LIT}{EXP_LIT}          { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%e\n", yylval.as_float); return FLOAT_C; } } }
{INT_LIT}                       { if(!CheckCommentBody()){ if(CheckInt(&yylval.as_int)){ yTRACE(INT_C); dbprtf("%d\n", yylval.as_int); return INT_C;           } } }
{LR_FLOAT_LIT}                  { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%f\n", yylval.as_float); return FLOAT_C; } } }
{L_FLOAT_LIT}                   { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%f\n", yylval.as_float); return FLOAT_C; } } }
{R_FLOAT_LIT}                   { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); dbprtf("%f\n", yylval.as_float); return FLOAT_C; } } }

{INT_LIT}{EXP_LIT}{ID}          { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }
{LR_FLOAT_LIT}{EXP_LIT}{ID}     { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }
{L_FLOAT_LIT}{EXP_LIT}{ID}      { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }
{R_FLOAT_LIT}{EXP_LIT}{ID}      { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }
{INT_LIT}{ID}                   { if(!CheckCommentBody()){ yERROR("Integer literal should not be followed by identifier");                                       } }
{LR_FLOAT_LIT}{ID}              { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }
{L_FLOAT_LIT}{ID}               { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }
{R_FLOAT_LIT}{ID}               { if(!CheckCommentBody()){ yERROR("Float literal should not be followed by identifier");                                         } }

{WS}                            /* skip whitespace */
\n                              { yyline++;                                                                                                                        }
.                               { if(!CheckCommentBody()){ dbprtf("=> LINE %d : %s\n", yyline, yytext); yERROR("Unknown token");                                 } }

<IN_MULTILINE_COMMENT><<EOF>>   { BEGIN(INITIAL); yERROR("Comment block is not closed by */ until EOF"); errorOccurred = TRUE; yyterminate();                      }
%%

void dbprtf(const char* fmt, ...) {
#ifdef SCANNER_DEBUG
    va_list args;
    va_start(args, fmt);

    // Forward to vprintf()
    vprintf(fmt, args);

    va_end(args);
#endif
}

int CheckCommentHead(void) {
    if(inCommentBlock == TRUE) {
        yERROR("Expecting closing previous comment block with */ before opening new comment block with /*");
        return FALSE;
    }

    inCommentBlock = TRUE;
    BEGIN(IN_MULTILINE_COMMENT);
    dbprtf("MULTILINE COMMENT HEAD, LINE %d : %s\n", yyline, yytext);
    // '/*' is captured

    return TRUE;
}

int CheckCommentTail(void) {
    if(inCommentBlock == FALSE) {
        yERROR("Cannot find the beginning of this comment block with /*");
        return FALSE;
    }

    dbprtf("MULTILINE COMMENT TAIL, LINE %d : %s\n", yyline, yytext);
    BEGIN(INITIAL);
    inCommentBlock = FALSE;
    // '*/' is captured

    return TRUE;
}

int CheckCommentBody(void) {
    if(inCommentBlock == TRUE) {
        dbprtf("MULTILINE COMMENT BODY, LINE %d : %s\n", yyline, yytext);
    }
    return inCommentBlock;
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