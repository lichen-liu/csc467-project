
--------------------------------------------------------------------------
Warning-0: Read of potentially unassigned variable 'a' of type 'int' at Line 4:17 to Line 4:18.
      4:                  int a = a;
                                  ^ 
--------------------------------------------------------------------------
Warning-1: Self-initialization involves read of potentially unassigned variable 'a' of type 'int' at Line 4:9 to Line 4:19.
      4:                  int a = a;
                          ^^^^^^^^^^
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
        (DECLARATION a int 2)
    )
    (STATEMENTS
        (SCOPE
            (DECLARATIONS
                (DECLARATION a int a)
            )
            (STATEMENTS
            )
        )
    )
)
