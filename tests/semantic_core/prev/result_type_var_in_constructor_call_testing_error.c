
--------------------------------------------------------------------------
Error-0: Unmatched constructor parameters when calling constructor for 'vec4' at Line 4:13 to Line 4:52.
      4:              vec4a = vec4(3, vec4a[2], gl_FragColor[2], 2.0);
                              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 

Info: Expecting constructor arguments 'float, float, float, float'.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'vec4a' at Line 4:5 to Line 4:53, has expression of unknown type at Line 4:13 to Line 4:52 due to previous error(s).
      4:              vec4a = vec4(3, vec4a[2], gl_FragColor[2], 2.0);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-2: Read of potentially unassigned variable 'vec4a' of type 'vec4' at Line 4:21 to Line 4:26.
      4:              vec4a = vec4(3, vec4a[2], gl_FragColor[2], 2.0);
                                      ^^^^^                           
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
        (DECLARATION vec4a vec4)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE vec4a (CALL vec4 3 (INDEX float vec4a 2) (INDEX float gl_FragColor 2) 2.000000))
    )
)
Failed to compile
