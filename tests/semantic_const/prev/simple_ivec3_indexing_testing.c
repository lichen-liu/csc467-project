
--------------------------------------------------------------------------
Warning-0: Read of potentially unassigned variable 'intb' of type 'const int' at Line 4:22 to Line 4:26.
      4:              const int intb = intb;
                                       ^^^^ 
--------------------------------------------------------------------------
Warning-1: Self-initialization involves read of potentially unassigned variable 'intb' of type 'const int' at Line 4:5 to Line 4:27.
      4:              const int intb = intb;
                      ^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'ivec3a' of type 'const ivec3' successful at Line 2:5 to Line 2:41.
Info: Optimization for declaration of const-qualified symbol 'inta' of type 'const int' successful at Line 3:5 to Line 3:40.
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
        (DECLARATION ivec3a const ivec3 (CALL ivec3 3 6 9))
        (DECLARATION inta const int 20)
        (DECLARATION intb const int intb)
    )
    (STATEMENTS
    )
)
