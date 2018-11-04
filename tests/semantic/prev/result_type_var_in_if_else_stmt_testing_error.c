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
    )
    (STATEMENTS
        (ASSIGN bool gl_FragDepth true)
        (IF true (ASSIGN bool gl_FragDepth false))
    )
)

--------------------------------------------------------------------------
Error-0: Invalid variable assignment for Write-Only Result variable 'gl_FragDepth' in the scope of an if or else statement at Line 5:9 to Line 5:30.
      5:                  gl_FragDepth = false;
                          ^^^^^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'result bool gl_FragDepth'.
      5:                  gl_FragDepth = false;
                          ~~~~~~~~~~~~         
--------------------------------------------------------------------------
