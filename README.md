# csc467-project
CSC467 Compilers and Interpreters Project.

## Introduction
A toy compiler written in a mixture of C and C++ for a toy graphics language - MiniGLSL.

## Architecture

### Lexical Analyzer
Using `flex`.

### Parser
Using `Bison` parser generator.

### AST
Hand written Abstract Syntax Tree in purely Object-Oriented fashion using C++.

### Semantic Analysis
Hand written semantic analyzer with very user-friendly error and warning messages.

### Code Generation
Hand written code generator into target language of ARB fragment shader assembly.

## Running
Compile the code:
`make`

See available options:
`make man`

Run the compiler:
`./compiler467`

## Disclaimer
If you are a current student taking CSC467 Compiler course, and is doing a very similiar project.
Do not copy the code, as it is meaningless and wasting time. You do not want to waste a very
good opportunity to shape your c++ or c skill in writing a ~4000 lines project. Also the project
itself is incredibly interesting. You are very welcome to take a look at the code for overall
architecture, style and design choices being made. But do the work by yourself!

## Testing

### To Test LEXER
uncomment phase 1 in compiler467.c
``` bash
./compiler467 -Tn ./tests/lexer/*.c  
python ./tests/test_lexer.py
```

### To Test PARSER
``` bash
./compiler467 -Tp ./tests/parser/*.c
python ./tests/test_parser.py
```

### To Test SEMANTIC ANALYSIS
``` bash
./compiler467 -Tp ./tests/semantic_*/*.c
python ./tests/test_semantic.py
python ./tests/test_semantic_*.py
```

### To Test CODE GENERATION
```bash
./compiler467 -Dx ./tests/codegen/*.c
python ./tests/test_codegen.py
```

### To Run DEMO
```bash
./compiler467 -Dx -U ../Demos/Demo1/frag.txt ../Demos/Demo1/shader.frag
../Demos/Demo1/shader ../Demos/Demo1/frag.txt

./compiler467 -Dx -U ../Demos/Demo2/frag.txt ../Demos/Demo2/phong.frag
../Demos/Demo2/phong ../Demos/Demo2/frag.txt
```

### To Count Lines
``` bash
wc -l ast.cpp ast.h semantic.cpp semantic.h symbol.cpp symbol.h codegen.cpp codegen.h
```

### To Commit
'make clean' first!
