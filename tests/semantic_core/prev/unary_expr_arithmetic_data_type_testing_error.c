
--------------------------------------------------------------------------
Error-0: Operand in unary expression at Line 11:14 to Line 11:18 has non-compatible type at Line 11:15 to Line 11:18.
     11:              floata = !1.3;
                               ^^^^ 

Info: Expecting boolean type on right-hand side of operator '!'
--------------------------------------------------------------------------
Error-1: Variable assignment for 'floata' at Line 11:5 to Line 11:19, has expression of unknown type at Line 11:14 to Line 11:18 due to previous error(s).
     11:              floata = !1.3;
                      ^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Operand in unary expression at Line 12:14 to Line 12:21 has non-compatible type at Line 12:15 to Line 12:21.
     12:              floata = !floatb;
                               ^^^^^^^ 

Info: Expecting boolean type on right-hand side of operator '!'
--------------------------------------------------------------------------
Error-3: Variable assignment for 'floata' at Line 12:5 to Line 12:22, has expression of unknown type at Line 12:14 to Line 12:21 due to previous error(s).
     12:              floata = !floatb;
                      ^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-4: Operand in unary expression at Line 16:14 to Line 16:21 has non-compatible type at Line 16:15 to Line 16:21.
     16:              ivec3a = !ivec3b;
                               ^^^^^^^ 

Info: Expecting boolean type on right-hand side of operator '!'
--------------------------------------------------------------------------
Error-5: Variable assignment for 'ivec3a' at Line 16:5 to Line 16:22, has expression of unknown type at Line 16:14 to Line 16:21 due to previous error(s).
     16:              ivec3a = !ivec3b;
                      ^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-6: Operand in unary expression at Line 18:14 to Line 18:33 has non-compatible type at Line 18:15 to Line 18:33.
     18:              ivec3a = !ivec3(10, 20, -30);
                               ^^^^^^^^^^^^^^^^^^^ 

Info: Expecting boolean type on right-hand side of operator '!'
--------------------------------------------------------------------------
Error-7: Variable assignment for 'ivec3a' at Line 18:5 to Line 18:34, has expression of unknown type at Line 18:14 to Line 18:33 due to previous error(s).
     18:              ivec3a = !ivec3(10, 20, -30);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-8: Read of potentially unassigned variable 'floatb' of type 'float' at Line 9:15 to Line 9:21.
      9:              floata = -floatb;
                                ^^^^^^ 
--------------------------------------------------------------------------
Warning-9: Read of potentially unassigned variable 'floatb' of type 'float' at Line 12:15 to Line 12:21.
     12:              floata = !floatb;
                                ^^^^^^ 
--------------------------------------------------------------------------
Warning-10: Read of potentially unassigned variable 'ivec3b' of type 'ivec3' at Line 15:15 to Line 15:21.
     15:              ivec3a = -ivec3b;
                                ^^^^^^ 
--------------------------------------------------------------------------
Warning-11: Read of potentially unassigned variable 'ivec3b' of type 'ivec3' at Line 16:15 to Line 16:21.
     16:              ivec3a = !ivec3b;
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
        (DECLARATION floata float)
        (DECLARATION floatb float)
        (DECLARATION ivec3a ivec3 (CALL ivec3 10 20 30))
        (DECLARATION ivec3b ivec3)
    )
    (STATEMENTS
        (ASSIGN float floata (UNARY float - floatb))
        (ASSIGN float floata (UNARY float - 1.200000))
        (ASSIGN ANY_TYPE floata (UNARY ANY_TYPE ! 1.300000))
        (ASSIGN ANY_TYPE floata (UNARY ANY_TYPE ! floatb))
        (ASSIGN ivec3 ivec3a (UNARY ivec3 - ivec3b))
        (ASSIGN ANY_TYPE ivec3a (UNARY ANY_TYPE ! ivec3b))
        (ASSIGN ivec3 ivec3a (UNARY ivec3 - (CALL ivec3 10 20 (UNARY int - 30))))
        (ASSIGN ANY_TYPE ivec3a (UNARY ANY_TYPE ! (CALL ivec3 10 20 (UNARY int - 30))))
    )
)
Failed to compile
