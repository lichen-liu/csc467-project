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
        (ASSIGN ANY_TYPE (INDEX float gl_Light_Half 1) vec4a)
    )
)

--------------------------------------------------------------------------
Error-0: Invalid variable assignment for Read-Only variable 'gl_Light_Half[1]' at Line 3:5 to Line 3:30.
      3:              gl_Light_Half[1] = vec4a;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'uniform float gl_Light_Half'.
      3:              gl_Light_Half[1] = vec4a;
                      ~~~~~~~~~~~~~~~~         
--------------------------------------------------------------------------
Error-1: Invalid variable assignment for 'gl_Light_Half[1]' at Line 3:5 to Line 3:30, has expression of non-compatible type 'vec4' at Line 3:24 to Line 3:29.
      3:              gl_Light_Half[1] = vec4a;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^

Info: Variable 'gl_Light_Half[1]' at Line 3:5 to Line 3:21 has type 'uniform float', and is a predefined variable.
--------------------------------------------------------------------------
