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
        (DECLARATION b float)
        (DECLARATION vec4a vec4)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE b (CALL dp3 vec4a gl_FragColor))
    )
)

--------------------------------------------------------------------------
Error-0: Arguments in function call at Line 5:9 to Line 5:33 has write-only Result type.
      5:              b = dp3(vec4a, gl_FragColor);
                          ^^^^^^^^^^^^^^^^^^^^^^^^ 

Info: The first write-only Result variable is 'gl_FragColor':
      5:              b = dp3(vec4a, gl_FragColor);
                                     ~~~~~~~~~~~~  
--------------------------------------------------------------------------
Error-1: Variable assignment for 'b' at Line 5:5 to Line 5:34, has expression of unknown type at Line 5:9 to Line 5:33 due to previous error(s).
      5:              b = dp3(vec4a, gl_FragColor);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
