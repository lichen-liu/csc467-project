(SCOPE
    (DECLARATIONS
        (DECLARATION gl_FragColor result vec4)
        (DECLARATION gl_FragDepth result bool)
        (DECLARATION gl_FragCoord result vec4)
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
