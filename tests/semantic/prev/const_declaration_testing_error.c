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
        (DECLARATION intb int 2.000000)
        (DECLARATION intd constint inta)
        (DECLARATION inte constint 1)
        (DECLARATION intf constint inte)
        (DECLARATION intg constint (BINARY int + inta 1))
    )
    (STATEMENTS
    )
)

--------------------------------------------------------------------------
Error-0: Variable declaration of 'int intb' at Line 4:5 to Line 4:20, is initialized to a noncompatible type 'float' at Line 4:16 to Line 4:19.
      4:              int intb = 2.0;
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-1: Const qualified variable 'const int intd' is initialized to a non-const qualified expression at Line 5:22 to Line 5:26.
      5:              const int intd = inta;
                      ^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Const qualified variable 'const int intg' is initialized to a non-const qualified expression at Line 8:22 to Line 8:30.
      8:              const int intg = inta + 1;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
