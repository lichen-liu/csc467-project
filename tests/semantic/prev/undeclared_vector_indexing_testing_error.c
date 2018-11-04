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
        (DECLARATION a int (INDEX ANY_TYPE ivec3a 2))
    )
    (STATEMENTS
    )
)

--------------------------------------------------------------------------
Error-0: Missing declaration for symbol 'ivec3a' at Line 2:13 to Line 2:19.
      2:              int a = ivec3a[2];
                              ^^^^^^    
--------------------------------------------------------------------------
Error-1: Variable declaration of 'int a' at Line 2:5 to Line 2:23, is initialized to an unknown type at Line 2:13 to Line 2:22 due to previous error(s).
      2:              int a = ivec3a[2];
                      ^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
