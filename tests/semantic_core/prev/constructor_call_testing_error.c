
--------------------------------------------------------------------------
Error-0: Unmatched constructor parameters when calling constructor for 'int' at Line 11:12 to Line 11:20.
     11:              inta = int(1,2);
                             ^^^^^^^^ 

Info: Expecting constructor argument 'int'.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'inta' at Line 11:5 to Line 11:21, has expression of unknown type at Line 11:12 to Line 11:20 due to previous error(s).
     11:              inta = int(1,2);
                      ^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Unmatched constructor parameters when calling constructor for 'int' at Line 12:12 to Line 12:19.
     12:              inta = int(2.);
                             ^^^^^^^ 

Info: Expecting constructor argument 'int'.
--------------------------------------------------------------------------
Error-3: Variable assignment for 'inta' at Line 12:5 to Line 12:20, has expression of unknown type at Line 12:12 to Line 12:19 due to previous error(s).
     12:              inta = int(2.);
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-4: Unmatched constructor parameters when calling constructor for 'ivec2' at Line 13:14 to Line 13:28.
     13:              ivec2a = ivec2(1, 2, 3);
                               ^^^^^^^^^^^^^^ 

Info: Expecting constructor arguments 'int, int'.
--------------------------------------------------------------------------
Error-5: Variable assignment for 'ivec2a' at Line 13:5 to Line 13:29, has expression of unknown type at Line 13:14 to Line 13:28 due to previous error(s).
     13:              ivec2a = ivec2(1, 2, 3);
                      ^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-6: Unmatched constructor parameters when calling constructor for 'ivec2' at Line 14:14 to Line 14:26.
     14:              ivec2a = ivec2(1, 2.);
                               ^^^^^^^^^^^^ 

Info: Expecting constructor arguments 'int, int'.
--------------------------------------------------------------------------
Error-7: Variable assignment for 'ivec2a' at Line 14:5 to Line 14:27, has expression of unknown type at Line 14:14 to Line 14:26 due to previous error(s).
     14:              ivec2a = ivec2(1, 2.);
                      ^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-8: Unmatched constructor parameters when calling constructor for 'ivec2' at Line 15:14 to Line 15:22.
     15:              ivec2a = ivec2(1);
                               ^^^^^^^^ 

Info: Expecting constructor arguments 'int, int'.
--------------------------------------------------------------------------
Error-9: Variable assignment for 'ivec2a' at Line 15:5 to Line 15:23, has expression of unknown type at Line 15:14 to Line 15:22 due to previous error(s).
     15:              ivec2a = ivec2(1);
                      ^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-10: Unmatched constructor parameters when calling constructor for 'ivec2' at Line 16:14 to Line 16:28.
     16:              ivec2a = ivec2(true, 1);
                               ^^^^^^^^^^^^^^ 

Info: Expecting constructor arguments 'int, int'.
--------------------------------------------------------------------------
Error-11: Variable assignment for 'ivec2a' at Line 16:5 to Line 16:29, has expression of unknown type at Line 16:14 to Line 16:28 due to previous error(s).
     16:              ivec2a = ivec2(true, 1);
                      ^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION gl_FragColor result vec4)
        (DECLARATION gl_FragDepth result bool)
        (DECLARATION gl_FragCoord attribute vec4)
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
        (DECLARATION inta int (CALL int 1))
        (DECLARATION floata float (CALL float 2.000000))
        (DECLARATION boola bool (CALL bool true))
        (DECLARATION ivec2a ivec2 (CALL ivec2 1 inta))
        (DECLARATION bvec3a bvec3 (CALL bvec3 true false boola))
        (DECLARATION vec4a vec4 (CALL vec4 1.000000 2.000000 floata floata))
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE inta (CALL int 1 2))
        (ASSIGN ANY_TYPE inta (CALL int 2.000000))
        (ASSIGN ANY_TYPE ivec2a (CALL ivec2 1 2 3))
        (ASSIGN ANY_TYPE ivec2a (CALL ivec2 1 2.000000))
        (ASSIGN ANY_TYPE ivec2a (CALL ivec2 1))
        (ASSIGN ANY_TYPE ivec2a (CALL ivec2 true 1))
    )
)
Failed to compile
