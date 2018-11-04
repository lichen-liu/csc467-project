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
        (DECLARATION inta int)
        (DECLARATION inta constint 2)
    )
    (STATEMENTS
    )
)

--------------------------------------------------------------------------
Error-0: Duplicate declaration of 'const int inta' at Line 4:5 to Line 4:24. Previously declared at Line 3:5 to Line 3:14.
      4:              const int inta = 2;
                      ^^^^^^^^^^^^^^^^^^^

Info: Previously declared here:
      3:              int inta;
                      ~~~~~~~~~
--------------------------------------------------------------------------
