
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 2:22 to Line 2:33 have non-compatible type.
      2:              const int inta = 2 + env2[2];
                                       ^^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-1: Variable declaration of 'const int inta' at Line 2:5 to Line 2:34, is initialized to an unknown type at Line 2:22 to Line 2:33 due to previous error(s).
      2:              const int inta = 2 + env2[2];
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'intb' of type 'const int' successful at Line 3:5 to Line 3:24.
Info: Optimization for declaration of const-qualified symbol 'intc' of type 'const int' successful at Line 4:5 to Line 4:39.
(SCOPE
    (DECLARATIONS
        (DECLARATION gl_FragColor result vec4)
        (DECLARATION gl_FragDepth result bool)
        (DECLARATION gl_FragCoord attribute vec4)
        (DECLARATION gl_TexCoord attribute vec4)
        (DECLARATION gl_Color attribute vec4)
        (DECLARATION gl_Secondary attribute vec4)
        (DECLARATION gl_FogFragCoord attribute vec4)
        (DECLARATION gl_Light_Half uniform vec4)
        (DECLARATION gl_Light_Ambient uniform vec4)
        (DECLARATION gl_Material_Shininess uniform vec4)
        (DECLARATION env1 uniform vec4)
        (DECLARATION env2 uniform vec4)
        (DECLARATION env3 uniform vec4)
        (DECLARATION inta const int (BINARY ANY_TYPE + 2 (INDEX float env2 2)))
        (DECLARATION intb const int 3)
        (DECLARATION intc const int 12)
    )
    (STATEMENTS
    )
)
Failed to compile
