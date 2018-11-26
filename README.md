# csc467-project
CSC467 Project

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
./compiler467 -Dx
```

### To Count Lines
``` bash
wc -l ast.cpp ast.h semantic.cpp semantic.h symbol.cpp symbol.h codegen.cpp codegen.h
```

### To Commit
'make clean' first!
