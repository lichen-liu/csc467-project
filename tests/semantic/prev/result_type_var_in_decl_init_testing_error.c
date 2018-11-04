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
        (DECLARATION vec4a vec4)
        (DECLARATION vec4b vec4 (BINARY ANY_TYPE + vec4a gl_FragColor))
    )
    (STATEMENTS
    )
)

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
