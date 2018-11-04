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
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE vec4a (CALL vec4 3 (INDEX float vec4a 2) (INDEX float gl_FragColor 2) 2.000000))
    )
)

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
