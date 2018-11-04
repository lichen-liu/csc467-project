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
