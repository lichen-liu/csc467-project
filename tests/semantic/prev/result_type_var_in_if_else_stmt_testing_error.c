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
