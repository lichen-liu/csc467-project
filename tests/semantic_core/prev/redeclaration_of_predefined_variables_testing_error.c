
--------------------------------------------------------------------------
Error-0: Duplicate declaration of 'vec4 gl_TexCoord' at Line 2:5 to Line 2:22.
      2:              vec4 gl_TexCoord;
                      ^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'attribute vec4 gl_TexCoord'.
--------------------------------------------------------------------------
Error-1: Duplicate declaration of 'vec4 gl_Color' at Line 4:9 to Line 4:23.
      4:                  vec4 gl_Color;
                          ^^^^^^^^^^^^^^

Info: Predefined Variable: 'attribute vec4 gl_Color'.
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
        (DECLARATION gl_TexCoord vec4)
    )
    (STATEMENTS
        (SCOPE
            (DECLARATIONS
                (DECLARATION gl_Color vec4)
            )
            (STATEMENTS
            )
        )
    )
)
Failed to compile
