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
int CheckID(char *id_value);

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

[A-Za-z_][A-Za-z0-9_]*                  { if(!CheckCommentBody()){ if(CheckID(yylval.as_id)){ yTRACE(ID); printf("%s\n", yylval.as_id); return ID;                     } } }

{DIGIT}+[eE]{SIGN}?{DIGIT}+             { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%e\n", yylval.as_float); return FLOAT_C; } } }
{DIGIT}+\.{DIGIT}+[eE]{SIGN}?{DIGIT}+   { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%e\n", yylval.as_float); return FLOAT_C; } } }
{DIGIT}+\.[eE]{SIGN}?{DIGIT}+           { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%e\n", yylval.as_float); return FLOAT_C; } } }
\.{DIGIT}+[eE]{SIGN}?{DIGIT}+           { if(!CheckCommentBody()){ if(CheckFloat(&yylval.as_float)){ yTRACE(FLOAT_C); printf("%e\n", yylval.as_float); return FLOAT_C; } } }

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

    return TRUE;
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

    return TRUE;
}

int CheckCommentBody(void) {
    if(inCommentBlock == TRUE) {
        //
        fprintf(traceFile, "MULTILINE COMMENT BODY, LINE %d : %s\n", yyline, yytext);
        //
    }
    return inCommentBlock;
}

int CheckInt(int *int_value) {
    char *pend;
    long int integer_value = strtol(yytext, &pend, 10);

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

    /* bound-check */
    if(double_value < MIN_FLOAT) {
        yERROR("Float literal is not within the legal range");
        *float_value = MIN_FLOAT;
        return FALSE;
    } else if(double_value > MAX_FLOAT) {
        yERROR("Float literal is not within the legal range");
        *float_value = MAX_FLOAT;
        return FALSE;
    }

    *float_value = double_value; // cast from double to float
    return TRUE;
}

int CheckID(char *id_value) {
    /* identifier length check */
    if(strlen(yytext) >= MAX_IDENTIFIER) {
        /* identifier + '\0' <= MAX_IDENTIFIER */
        yERROR("Identifier has a illegal length");
        strcpy(id_value, "");
        return FALSE;
    }

    strcpy(id_value, yytext);
    return TRUE;
}