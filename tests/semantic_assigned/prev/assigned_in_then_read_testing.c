Info: Optimization for declaration of const-qualified symbol 'cond' of type 'const bool' successful at Line 4:5 to Line 4:28.
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
        (DECLARATION a int)
        (DECLARATION b int)
        (DECLARATION cond const bool true)
    )
    (STATEMENTS
        (IF cond (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN int a 1)
            )
        ) (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN int b 2)
            )
        ))
        (ASSIGN int b a)
    )
)
