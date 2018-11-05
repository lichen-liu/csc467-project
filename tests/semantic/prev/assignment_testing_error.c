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
        (DECLARATION inta int)
        (DECLARATION ivec3a ivec3)
        (DECLARATION vec3a vec3)
        (DECLARATION floata const float 2.000000)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE a (BINARY ANY_TYPE + 2 2.000000))
        (ASSIGN ANY_TYPE a 2)
        (ASSIGN ANY_TYPE inta (BINARY ANY_TYPE + 2 2.000000))
        (ASSIGN ANY_TYPE inta 2.000000)
        (ASSIGN ANY_TYPE (INDEX float vec3a 0) (BINARY ANY_TYPE + 2 2.000000))
        (ASSIGN ANY_TYPE (INDEX int ivec3a 1) 2.000000)
        (ASSIGN ANY_TYPE (INDEX ANY_TYPE vec3a 3) 3.000000)
        (ASSIGN ANY_TYPE floata 3.000000)
    )
)

--------------------------------------------------------------------------
Error-0: Missing declaration for symbol 'a' at Line 8:5 to Line 8:6.
      8:              a = 2 + 2.0;
                      ^           
--------------------------------------------------------------------------
Error-1: Operands in binary expression at Line 8:9 to Line 8:16 have non-compatible type.
      8:              a = 2 + 2.0;
                          ^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-2: Variable assignment for 'a' at Line 8:5 to Line 8:17, has unknown type on both sides of assignment operator due to pervious errors.
      8:              a = 2 + 2.0;
                      ^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-3: Missing declaration for symbol 'a' at Line 9:5 to Line 9:6.
      9:              a = 2;
                      ^     
--------------------------------------------------------------------------
Error-4: Variable assignment for 'a' at Line 9:5 to Line 9:11, has variable of unknown type at Line 9:5 to Line 9:6 due to previous error.
      9:              a = 2;
                      ^^^^^^
--------------------------------------------------------------------------
Error-5: Operands in binary expression at Line 10:12 to Line 10:19 have non-compatible type.
     10:              inta = 2 + 2.0;
                             ^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-6: Variable assignment for 'inta' at Line 10:5 to Line 10:20, has expression of unknown type at Line 10:12 to Line 10:19 due to previous error(s).
     10:              inta = 2 + 2.0;
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-7: Invalid variable assignment for 'inta' at Line 11:5 to Line 11:16, has expression of non-compatible type 'const float' at Line 11:12 to Line 11:15.
     11:              inta = 2.0;
                      ^^^^^^^^^^^

Info: Variable 'inta' at Line 11:5 to Line 11:9 has type 'int', and is declared at Line 3:5 to Line 3:14:
      3:              int inta;
                      ~~~~~~~~~
--------------------------------------------------------------------------
Error-8: Operands in binary expression at Line 13:16 to Line 13:23 have non-compatible type.
     13:              vec3a[0] = 2 + 2.0;
                                 ^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-9: Variable assignment for 'vec3a[0]' at Line 13:5 to Line 13:24, has expression of unknown type at Line 13:16 to Line 13:23 due to previous error(s).
     13:              vec3a[0] = 2 + 2.0;
                      ^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-10: Invalid variable assignment for 'ivec3a[1]' at Line 14:5 to Line 14:21, has expression of non-compatible type 'const float' at Line 14:17 to Line 14:20.
     14:              ivec3a[1] = 2.0;
                      ^^^^^^^^^^^^^^^^

Info: Variable 'ivec3a[1]' at Line 14:5 to Line 14:14 has type 'int', and is declared at Line 4:5 to Line 4:18:
      4:              ivec3 ivec3a;
                      ~~~~~~~~~~~~~
--------------------------------------------------------------------------
Error-11: Invalid indexing of vector variable 'vec3a' of type 'vec3' at Line 16:5 to Line 16:13.
     16:              vec3a[3] = 3.0;
                      ^^^^^^^^       

Info: Expecting an index value between 0 to 2.
--------------------------------------------------------------------------
Error-12: Variable assignment for 'vec3a[3]' at Line 16:5 to Line 16:20, has variable of unknown type at Line 16:5 to Line 16:13 due to previous error.
     16:              vec3a[3] = 3.0;
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-13: Invalid variable assignment for const-qualified variable 'floata' at Line 18:5 to Line 18:18.
     18:              floata = 3.0;
                      ^^^^^^       

Info: Variable 'floata' at Line 18:5 to Line 18:11 has type 'const float', and is declared at Line 6:5 to Line 6:30:
      6:              const float floata = 2.0;
                      ~~~~~~~~~~~~~~~~~~~~~~~~~
--------------------------------------------------------------------------
