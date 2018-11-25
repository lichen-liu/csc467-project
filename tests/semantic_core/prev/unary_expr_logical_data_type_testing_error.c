
--------------------------------------------------------------------------
Error-0: Operand in unary expression at Line 11:13 to Line 11:18 has non-compatible type at Line 11:14 to Line 11:18.
     11:              boola = -true;
                              ^^^^^ 

Info: Expecting arithmetic type on right-hand side of operator '-'
--------------------------------------------------------------------------
Error-1: Variable assignment for 'boola' at Line 11:5 to Line 11:19, has expression of unknown type at Line 11:13 to Line 11:18 due to previous error(s).
     11:              boola = -true;
                      ^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Operand in unary expression at Line 15:14 to Line 15:21 has non-compatible type at Line 15:15 to Line 15:21.
     15:              bvec3a = -bvec3a;
                               ^^^^^^^ 

Info: Expecting arithmetic type on right-hand side of operator '-'
--------------------------------------------------------------------------
Error-3: Variable assignment for 'bvec3a' at Line 15:5 to Line 15:22, has expression of unknown type at Line 15:14 to Line 15:21 due to previous error(s).
     15:              bvec3a = -bvec3a;
                      ^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-4: Operand in unary expression at Line 16:14 to Line 16:39 has non-compatible type at Line 16:15 to Line 16:39.
     16:              bvec3a = -bvec3(true, false, true);
                               ^^^^^^^^^^^^^^^^^^^^^^^^^ 

Info: Expecting arithmetic type on right-hand side of operator '-'
--------------------------------------------------------------------------
Error-5: Variable assignment for 'bvec3a' at Line 16:5 to Line 16:40, has expression of unknown type at Line 16:14 to Line 16:39 due to previous error(s).
     16:              bvec3a = -bvec3(true, false, true);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-6: Read of potentially unassigned variable 'boolb' of type 'bool' at Line 9:14 to Line 9:19.
      9:              boola = !boolb;
                               ^^^^^ 
--------------------------------------------------------------------------
Warning-7: Read of potentially unassigned variable 'bvec3b' of type 'bvec3' at Line 14:15 to Line 14:21.
     14:              bvec3a = !bvec3b;
                                ^^^^^^ 
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
        (DECLARATION boola bool)
        (DECLARATION boolb bool)
        (DECLARATION bvec3a bvec3 (CALL bvec3 true false true))
        (DECLARATION bvec3b bvec3)
    )
    (STATEMENTS
        (ASSIGN bool boola (UNARY bool ! boolb))
        (ASSIGN bool boola (UNARY bool ! false))
        (ASSIGN ANY_TYPE boola (UNARY ANY_TYPE - true))
        (ASSIGN bvec3 bvec3a (UNARY bvec3 ! bvec3b))
        (ASSIGN ANY_TYPE bvec3a (UNARY ANY_TYPE - bvec3a))
        (ASSIGN ANY_TYPE bvec3a (UNARY ANY_TYPE - (CALL bvec3 true false true)))
        (ASSIGN bvec3 bvec3a (UNARY bvec3 ! (CALL bvec3 true false true)))
    )
)
Failed to compile
