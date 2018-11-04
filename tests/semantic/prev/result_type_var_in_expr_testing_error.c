(SCOPE
    (DECLARATIONS
        (DECLARATION gl_FragColor resultvec4)
        (DECLARATION gl_FragDepth resultbool)
        (DECLARATION gl_FragCoord resultvec4)
        (DECLARATION gl_TexCoord attributevec4)
        (DECLARATION gl_Color attributevec4)
        (DECLARATION gl_Secondary attributevec4)
        (DECLARATION gl_FogFragCoord attributevec4)
        (DECLARATION gl_Light_Half uniformvec4)
        (DECLARATION gl_Light_Ambient uniformvec4)
        (DECLARATION gl_Material_Shininess uniformvec4)
        (DECLARATION env1 uniformvec4)
        (DECLARATION env2 uniformvec4)
        (DECLARATION env3 uniformvec4)
        (DECLARATION gl_FragColor vec4)
        (DECLARATION a vec4)
        (DECLARATION b vec4)
        (DECLARATION c vec3)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE a (BINARY ANY_TYPE + (UNARY ANY_TYPE - (UNARY ANY_TYPE - (UNARY ANY_TYPE - (UNARY ANY_TYPE - (UNARY ANY_TYPE - gl_FragColor))))) gl_FragCoord))
        (ASSIGN ANY_TYPE b (BINARY ANY_TYPE + (BINARY ANY_TYPE + c a) (BINARY ANY_TYPE + (INDEX float gl_FragCoord 2) c)))
    )
)

--------------------------------------------------------------------------
Error-0: Duplicate declaration of 'vec4 gl_FragColor' at Line 2:5 to Line 2:23. Previously declared at Line 0:0 to Line 0:0.
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
Error-4: Operands in binary expression at Line 8:18 to Line 8:37 has write-only Result type.
      8:              b = c + a + (gl_FragCoord[2] + c);
                                   ^^^^^^^^^^^^^^^^^^^  

Info: The first write-only Result variable is 'gl_FragCoord[2]':
      8:              b = c + a + (gl_FragCoord[2] + c);
                                   ~~~~~~~~~~~~~~~      
--------------------------------------------------------------------------
Error-5: Operands in binary expression at Line 8:18 to Line 8:37 have non-compatible type.
      8:              b = c + a + (gl_FragCoord[2] + c);
                                   ^^^^^^^^^^^^^^^^^^^  

Info: Expecting operands on both sides of operator '+' to have arithmetic type and same order, but they are 'float' and 'vec3'.
--------------------------------------------------------------------------
Error-6: Variable assignment for 'b' at Line 8:5 to Line 8:39, has expression of unknown type at Line 8:9 to Line 8:38 due to previous error(s).
      8:              b = c + a + (gl_FragCoord[2] + c);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
