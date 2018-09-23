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

int CheckInt(void);
float CheckFloat(void);

int inCommentBlock = FALSE;
int CheckCommentHead(void);
int CheckCommentTail(void);
/* TRUE if in multi-line comment block */
int CheckCommentBody(void);

%}
%option noyywrap

WS                          [ \t]
DIGIT                       [0-9]
SIGN                        [\+\-]

%%
"/*".*"*/"                  { if(!CheckCommentBody()){ fprintf(traceFile, "SINGLELINE COMMENT, LINE %d : %s\n", yyline, yytext);                             } }
"/*".*                      { CheckCommentHead();                                                                                                              }
.*"*/"                      { CheckCommentTail();                                                                                                              }

"if"                        { if(!CheckCommentBody()){ yTRACE(IF); return IF; } }
"else"                      { if(!CheckCommentBody()){ yTRACE(ELSE); return ELSE;                                                                            } }

{SIGN}?{DIGIT}+             { if(!CheckCommentBody()){ yTRACE(INT_C); yylval.as_int = CheckInt(); printf("%d\n", yylval.as_int); return INT_C;               } }
{SIGN}?{DIGIT}+\.{DIGIT}+   { if(!CheckCommentBody()){ yTRACE(FLOAT_C); yylval.as_float = CheckFloat(); printf("%f\n", yylval.as_float); return FLOAT_C;     } }
{SIGN}?{DIGIT}+\.           { if(!CheckCommentBody()){ yTRACE(FLOAT_C); yylval.as_float = CheckFloat(); printf("%f\n", yylval.as_float); return FLOAT_C;     } }
{SIGN}?\.{DIGIT}+           { if(!CheckCommentBody()){ yTRACE(FLOAT_C); yylval.as_float = CheckFloat(); printf("%f\n", yylval.as_float); return FLOAT_C;     } }

{WS}                        /* skip whitespace */
\n                          { yyline++;                                                                                                                        }
.                           { if(!CheckCommentBody()){ printf("=> LINE %d : %s\n", yyline, yytext); yERROR("Unknown token");                                  } }
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

int CheckInt(void) {
    char *pend;
    long int integer_value = strtol(yytext, &pend, 10);

    /* @todo bound-check */
    return integer_value; // cast from long to int
}

float CheckFloat(void) {
    char *pend;
    double float_value = strtod(yytext, &pend);

    /* @todo bound-check */
    return float_value; // cast from double to float
}