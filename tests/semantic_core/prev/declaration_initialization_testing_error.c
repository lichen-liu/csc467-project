
--------------------------------------------------------------------------
Error-0: Variable declaration of 'int intb' at Line 4:5 to Line 4:20, is initialized to a noncompatible type 'float' at Line 4:16 to Line 4:19.
      4:              int intb = 2.0;
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-1: Operands in binary expression at Line 5:16 to Line 5:26 have non-compatible type.
      5:              int intc = inta + 3.0;
                                 ^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-2: Variable declaration of 'int intc' at Line 5:5 to Line 5:27, is initialized to an unknown type at Line 5:16 to Line 5:26 due to previous error(s).
      5:              int intc = inta + 3.0;
                      ^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-3: Missing declaration for symbol 'inte' at Line 6:16 to Line 6:20.
      6:              int intd = inte;
                                 ^^^^ 
--------------------------------------------------------------------------
Error-4: Variable declaration of 'int intd' at Line 6:5 to Line 6:21, is initialized to an unknown type at Line 6:16 to Line 6:20 due to previous error(s).
      6:              int intd = inte;
                      ^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-5: Read of potentially unassigned variable 'inta' of type 'int' at Line 5:16 to Line 5:20.
      5:              int intc = inta + 3.0;
                                 ^^^^       
--------------------------------------------------------------------------
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
        (DECLARATION inta int)
        (DECLARATION intb int 2.000000)
        (DECLARATION intc int (BINARY ANY_TYPE + inta 3.000000))
        (DECLARATION intd int inte)
    )
    (STATEMENTS
    )
)
Failed to compile
