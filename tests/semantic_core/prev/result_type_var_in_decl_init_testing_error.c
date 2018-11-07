
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 3:18 to Line 3:38 has write-only Result type.
      3:              vec4 vec4b = vec4a + gl_FragColor;
                                   ^^^^^^^^^^^^^^^^^^^^ 

Info: The first write-only Result variable is 'gl_FragColor':
      3:              vec4 vec4b = vec4a + gl_FragColor;
                                           ~~~~~~~~~~~~ 
--------------------------------------------------------------------------
Error-1: Variable declaration of 'vec4 vec4b' at Line 3:5 to Line 3:39, is initialized to an unknown type at Line 3:18 to Line 3:38 due to previous error(s).
      3:              vec4 vec4b = vec4a + gl_FragColor;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
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
        (DECLARATION vec4a vec4 (CALL vec4 1.000000 2.000000 3.000000 4.000000))
        (DECLARATION vec4b vec4 (BINARY ANY_TYPE + vec4a gl_FragColor))
    )
    (STATEMENTS
    )
)
Failed to compile
