###########################################################################
# Makefile
#
# makefile for compiler  - CSC467F course project compiler
###########################################################################

###########################################################################
# This make file provides the following targets
#
# make  compiler467  Build the complete compiler
# make  lex.yy.c     Build the scanner
# make  parser.c     Build the parser C code 
# make  parser.tab.h Build the parser parser.tab.h header
# make  ast          Build the AST module
# make  semantics    Build the semantics module
# make  codegen      Build the code generator module
# make  symbol       Build the symbol table module
# make  machine      Build the machine interpreter module
###########################################################################

###########################################################################
# Compiler and Compiler Flags
###########################################################################
CC      =g++
CFLAGS  =-g -O0 -Wall

LEX     =flex
LEXFLAGS=-l

YACC    =bison
YFLAGS  =-dtvy

###########################################################################
#	Some define files that make up the compiler source.
#	Add more object files here for the subsequent modules of 
#	the compiler that you will program.
###########################################################################
#LEXER_OBJ =handlex.o
LEXER_OBJ =scanner.o
PARSER_OBJ=parser.o
AST_OBJ   =ast.o semantic.o symbol.o
CODE_OBJ  =codegen.o  
OBJs      =compiler467.o globalvars.o $(LEXER_OBJ) \
           $(PARSER_OBJ) $(AST_OBJ) $(CODE_OBJ)

###########################################################################
#	PHONY rules
###########################################################################
.PHONY: all clean man
all: compiler467
clean:
	@$(RM) compiler467 $(OBJs) lex.yy.c parser.tab.h parser.c y.output
man:
	@nroff -man compiler467.man | less

###########################################################################
#	Dependencies for the compiler
###########################################################################
compiler467: ${OBJs}
${OBJs}:     common.h 
lex.yy.c:    scanner.l
	$(LEX) $(LEXFLAGS) $<
$(LEXER_OBJ): parser.tab.h
parser.tab.h: parser.c
	mv y.tab.h parser.tab.h
