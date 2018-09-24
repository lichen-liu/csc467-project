%{
/**********************************************************************
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include "string.h"


#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int yyline = 1;

/* TRUE if valid */
int CheckInt(int *int_value);
int CheckFloat(float *float_value);

int inCommentBlock = FALSE;
int CheckCommentHead(void);
int CheckCommentTail(void);
/* TRUE if in multi-line comment block */
int CheckCommentBody(void);

%}
%option noyywrap

WS                                      [ \t]
DIGIT                                   [0-9]
SIGN                                    [\+\-]

%%
"/*".*"*/"                              { if(!CheckCommentBody()){ fprintf(traceFile, "SINGLELINE COMMENT, LINE %d : %s\n", yyline, yytext);                             } }
"/*".*                                  { CheckCommentHead();                                                                                                              }
.*"*/"                                  { CheckCommentTail();                                                                                                              }

"if"                                    { if(!CheckCommentBody()){ yTRACE(IF); return IF;                                                                                } }
"else"                                  { if(!CheckCommentBody()){ yTRACE(ELSE); return ELSE;                                                                            } }



{DIGIT}+[eE]{SIGN}?{DIGIT}+             { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }
{DIGIT}+\.{DIGIT}+[eE]{SIGN}?{DIGIT}+   { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }
{DIGIT}+\.[eE]{SIGN}?{DIGIT}+           { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }
\.{DIGIT}+[eE]{SIGN}?{DIGIT}+           { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }

{DIGIT}+                                { if(!CheckCommentBody()){ if(CheckInt(&yylval.as_int)){ yTRACE(INT_C); printf("%d\n", yylval.as_int); return INT_C;           } } }
{DIGIT}+\.{DIGIT}+                      { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }
{DIGIT}+\.                              { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }
\.{DIGIT}+                              { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%f\n", yylval.as_float); return FLOAT_C; } } }

{WS}                                    /* skip whitespace */
\n                                      { yyline++;                                                                                                                        }
.                                       { if(!CheckCommentBody()){ printf("=> LINE %d : %s\n", yyline, yytext); yERROR("Unknown token");                                 } }
%%


int CheckCommentHead(void) {
    if(inCommentBlock == TRUE) {
        yERROR("Expecting closing previous comment block with */ before opening new comment block with /*");
    }

    inCommentBlock = TRUE;
    //
    fprintf(traceFile, "MULTILINE COMMENT HEAD, LINE %d : %s\n", yyline, yytext);
    //
    // '/*' is captured

    return 0;
}

int CheckCommentTail(void) {
    if(inCommentBlock == FALSE) {
        yERROR("Cannot find the beginning of this comment block with /*");
    }

    //
    fprintf(traceFile, "MULTILINE COMMENT TAIL, LINE %d : %s\n", yyline, yytext);
    //
    inCommentBlock = FALSE;
    // '*/' is captured

    return 0;
}

int CheckCommentBody(void) {
    if(inCommentBlock == TRUE) {
        //
        fprintf(traceFile, "MULTI LINE COMMENT BODY, LINE %d : %s\n", yyline, yytext);
        //
    }
    return inCommentBlock;
}

int CheckInt(int *int_value) {
    char *pend;
    long int integer_value = strtol(yytext, &pend, 10);

    /* @todo bound-check */
    
    *int_value = integer_value; // cast from long to int
    return TRUE;
}

int CheckFloat(float *float_value) {
    char *pend;
    double double_value = strtod(yytext, &pend);

    /* @todo bound-check */

    *float_value = double_value; // cast from double to float
    return TRUE;
}