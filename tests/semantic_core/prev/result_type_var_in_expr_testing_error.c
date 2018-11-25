
--------------------------------------------------------------------------
Error-0: Duplicate declaration of 'vec4 gl_FragColor' at Line 2:5 to Line 2:23.
      2:              vec4 gl_FragColor;
                      ^^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'result vec4 gl_FragColor'.
--------------------------------------------------------------------------
Error-1: Operand in unary expression at Line 7:13 to Line 7:26 has write-only Result type at Line 7:14 to Line 7:26.
      7:              a = -----gl_FragColor + gl_FragCoord;
                              ^^^^^^^^^^^^^                

Info: The first write-only Result variable is 'gl_FragColor':
      7:              a = -----gl_FragColor + gl_FragCoord;
                               ~~~~~~~~~~~~                
--------------------------------------------------------------------------
Error-2: Variable assignment for 'a' at Line 7:5 to Line 7:42, has expression of unknown type at Line 7:9 to Line 7:41 due to previous error(s).
      7:              a = -----gl_FragColor + gl_FragCoord;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-3: Operands in binary expression at Line 8:9 to Line 8:14 have non-compatible type.
      8:              b = c + a + (gl_FragCoord[2] + c);
                          ^^^^^                         

Info: Expecting vector operands on both sides of operator to have same order, but they are 'vec3' and 'vec4'.
--------------------------------------------------------------------------
Error-4: Operands in binary expression at Line 8:18 to Line 8:37 have non-compatible type.
      8:              b = c + a + (gl_FragCoord[2] + c);
                                   ^^^^^^^^^^^^^^^^^^^  

Info: Expecting operands on both sides of operator '+' to have arithmetic type and same order, but they are 'float' and 'vec3'.
--------------------------------------------------------------------------
Error-5: Variable assignment for 'b' at Line 8:5 to Line 8:39, has expression of unknown type at Line 8:9 to Line 8:38 due to previous error(s).
      8:              b = c + a + (gl_FragCoord[2] + c);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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
        (DECLARATION gl_FragColor vec4)
        (DECLARATION a vec4 (CALL vec4 2.000000 2.000000 2.000000 3.000000))
        (DECLARATION b vec4 (CALL vec4 2.000000 2.000000 2.000000 3.000000))
        (DECLARATION c vec3 (CALL vec3 2.000000 2.000000 3.000000))
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE a (BINARY ANY_TYPE + (UNARY ANY_TYPE - (UNARY ANY_TYPE - (UNARY ANY_TYPE - (UNARY ANY_TYPE - (UNARY ANY_TYPE - gl_FragColor))))) gl_FragCoord))
        (ASSIGN ANY_TYPE b (BINARY ANY_TYPE + (BINARY ANY_TYPE + c a) (BINARY ANY_TYPE + (INDEX float gl_FragCoord 2) c)))
    )
)
Failed to compile
