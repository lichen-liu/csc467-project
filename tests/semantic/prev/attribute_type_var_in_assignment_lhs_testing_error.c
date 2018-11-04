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
        (DECLARATION vec4a vec4)
    )
    (STATEMENTS
        (ASSIGN float (INDEX float gl_Color 0) 2.000000)
    )
)

--------------------------------------------------------------------------
Error-0: Invalid variable assignment for Read-Only variable 'gl_Color[0]' at Line 3:5 to Line 3:23.
      3:              gl_Color[0] = 2.0;
                      ^^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'attribute float gl_Color'.
      3:              gl_Color[0] = 2.0;
                      ~~~~~~~~~~~       
--------------------------------------------------------------------------
