
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 25:14 to Line 25:29 have non-compatible type.
     25:              ivec3a = ivec3a + ivec4a;
                               ^^^^^^^^^^^^^^^ 

Info: Expecting vector operands on both sides of operator to have same order, but they are 'ivec3' and 'ivec4'.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'ivec3a' at Line 25:5 to Line 25:30, has expression of unknown type at Line 25:14 to Line 25:29 due to previous error(s).
     25:              ivec3a = ivec3a + ivec4a;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Operands in binary expression at Line 28:13 to Line 28:27 have non-compatible type.
     28:              vec3a = vec3a - floata;
                              ^^^^^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '-' to have arithmetic type and same order, but they are 'vec3' and 'float'.
--------------------------------------------------------------------------
Error-3: Variable assignment for 'vec3a' at Line 28:5 to Line 28:28, has expression of unknown type at Line 28:13 to Line 28:27 due to previous error(s).
     28:              vec3a = vec3a - floata;
                      ^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-4: Operands in binary expression at Line 29:12 to Line 29:25 have non-compatible type.
     29:              inta = ivec3c + inta;
                             ^^^^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have arithmetic type and same order, but they are 'ivec3' and 'int'.
--------------------------------------------------------------------------
Error-5: Variable assignment for 'inta' at Line 29:5 to Line 29:26, has expression of unknown type at Line 29:12 to Line 29:25 due to previous error(s).
     29:              inta = ivec3c + inta;
                      ^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-6: Operands in binary expression at Line 32:14 to Line 32:27 have non-compatible type.
     32:              floata = floata + inta;
                               ^^^^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'float' and 'int'.
--------------------------------------------------------------------------
Error-7: Variable assignment for 'floata' at Line 32:5 to Line 32:28, has expression of unknown type at Line 32:14 to Line 32:27 due to previous error(s).
     32:              floata = floata + inta;
                      ^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-8: Read of potentially unassigned variable 'inta' of type 'int' at Line 29:21 to Line 29:25.
     29:              inta = ivec3c + inta;
                                      ^^^^ 
--------------------------------------------------------------------------
Warning-9: Read of potentially unassigned variable 'inta' of type 'int' at Line 32:23 to Line 32:27.
     32:              floata = floata + inta;
                                        ^^^^ 
--------------------------------------------------------------------------
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
        (DECLARATION floata float 0.000000)
        (DECLARATION floatb float 1.000000)
        (DECLARATION floatc float 2.000000)
        (DECLARATION vec3a vec3 (CALL vec3 1.000000 2.000000 3.000000))
        (DECLARATION ivec3a ivec3 (CALL ivec3 10 20 30))
        (DECLARATION ivec3b ivec3 (CALL ivec3 1 2 3))
        (DECLARATION ivec3c ivec3 (CALL ivec3 4 5 6))
        (DECLARATION ivec4a ivec4 (CALL ivec4 5 6 7 8))
        (DECLARATION inta int)
    )
    (STATEMENTS
        (ASSIGN float floata (BINARY float + floata floatb))
        (ASSIGN float floata (BINARY float - 1.200000 floatb))
        (ASSIGN float floata (BINARY float + (UNARY float - 1.200000) floatc))
        (ASSIGN ivec3 ivec3a (BINARY ivec3 + ivec3c ivec3b))
        (ASSIGN ivec3 ivec3a (BINARY ivec3 - ivec3b (CALL ivec3 10 20 (UNARY int - 30))))
        (ASSIGN ANY_TYPE ivec3a (BINARY ANY_TYPE + ivec3a ivec4a))
        (ASSIGN ANY_TYPE vec3a (BINARY ANY_TYPE - vec3a floata))
        (ASSIGN ANY_TYPE inta (BINARY ANY_TYPE + ivec3c inta))
        (ASSIGN ANY_TYPE floata (BINARY ANY_TYPE + floata inta))
    )
)
Failed to compile
