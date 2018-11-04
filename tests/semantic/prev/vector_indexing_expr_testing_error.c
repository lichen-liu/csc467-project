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
        (DECLARATION ivec3a ivec3 (CALL ivec3 1 2 3))
        (DECLARATION b int)
    )
    (STATEMENTS
        (ASSIGN int b (INDEX int ivec3a 0))
        (ASSIGN int b (INDEX int ivec3a 1))
        (ASSIGN int b (INDEX int ivec3a 2))
        (ASSIGN ANY_TYPE b (INDEX ANY_TYPE ivec3a 3))
        (ASSIGN ANY_TYPE b (INDEX ANY_TYPE b 2))
    )
)

--------------------------------------------------------------------------
Error-0: Invalid indexing of vector variable 'ivec3a' of type 'ivec3' at Line 9:9 to Line 9:18.
      9:              b = ivec3a[3];
                          ^^^^^^^^^ 

Info: Expecting an index value between 0 to 2.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'b' at Line 9:5 to Line 9:19, has expression of unknown type at Line 9:9 to Line 9:18 due to previous error(s).
      9:              b = ivec3a[3];
                      ^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Invalid indexing of non-vector variable 'b' of type 'int' at Line 10:9 to Line 10:13.
     10:              b = b[2];
                          ^^^^ 
--------------------------------------------------------------------------
Error-3: Variable assignment for 'b' at Line 10:5 to Line 10:14, has expression of unknown type at Line 10:9 to Line 10:13 due to previous error(s).
     10:              b = b[2];
                      ^^^^^^^^^
--------------------------------------------------------------------------
