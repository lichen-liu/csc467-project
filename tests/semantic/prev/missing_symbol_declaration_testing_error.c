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
        (DECLARATION a int)
    )
    (STATEMENTS
        (ASSIGN int a 3)
        (ASSIGN ANY_TYPE a b)
    )
)

--------------------------------------------------------------------------
Error-0: Missing declaration for symbol'b' at Line 5:9 to Line 5:10.
      5:              a = b;
                          ^ 
--------------------------------------------------------------------------
Error-1: Variable assignment for 'a' at Line 5:5 to Line 5:11, has expression of unknown type at Line 5:9 to Line 5:10 due to previous error(s).
      5:              a = b;
                      ^^^^^^
--------------------------------------------------------------------------
