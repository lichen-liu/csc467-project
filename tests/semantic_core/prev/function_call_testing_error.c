
--------------------------------------------------------------------------
Error-0: Unmatched function parameters when calling function 'rsq' at Line 35:18 to Line 35:37.
     35:                  floata = rsq(floata, floata);
                                   ^^^^^^^^^^^^^^^^^^^ 

Info: Expecting function argument 'float' or 'int'.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'floata' at Line 35:9 to Line 35:38, has expression of unknown type at Line 35:18 to Line 35:37 due to previous error(s).
     35:                  floata = rsq(floata, floata);
                          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Unmatched function parameters when calling function 'rsq' at Line 36:18 to Line 36:28.
     36:                  floata = rsq(vec4a);
                                   ^^^^^^^^^^ 

Info: Expecting function argument 'float' or 'int'.
--------------------------------------------------------------------------
Error-3: Variable assignment for 'floata' at Line 36:9 to Line 36:29, has expression of unknown type at Line 36:18 to Line 36:28 due to previous error(s).
     36:                  floata = rsq(vec4a);
                          ^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-4: Unmatched function parameters when calling function 'dp3' at Line 37:18 to Line 37:36.
     37:                  floata = dp3(vec4a, ivec4a);
                                   ^^^^^^^^^^^^^^^^^^ 

Info: Expecting function arguments 'vec4, vec4' or 'vec3, vec3' or 'ivec4, ivec4' or 'ivec3, ivec3'.
--------------------------------------------------------------------------
Error-5: Variable assignment for 'floata' at Line 37:9 to Line 37:37, has expression of unknown type at Line 37:18 to Line 37:36 due to previous error(s).
     37:                  floata = dp3(vec4a, ivec4a);
                          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-6: Unmatched function parameters when calling function 'dp3' at Line 38:18 to Line 38:35.
     38:                  floata = dp3(vec4a, vec3a);
                                   ^^^^^^^^^^^^^^^^^ 

Info: Expecting function arguments 'vec4, vec4' or 'vec3, vec3' or 'ivec4, ivec4' or 'ivec3, ivec3'.
--------------------------------------------------------------------------
Error-7: Variable assignment for 'floata' at Line 38:9 to Line 38:36, has expression of unknown type at Line 38:18 to Line 38:35 due to previous error(s).
     38:                  floata = dp3(vec4a, vec3a);
                          ^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-8: Unmatched function parameters when calling function 'dp3' at Line 39:18 to Line 39:35.
     39:                  floata = dp3(ivec4a, inta);
                                   ^^^^^^^^^^^^^^^^^ 

Info: Expecting function arguments 'vec4, vec4' or 'vec3, vec3' or 'ivec4, ivec4' or 'ivec3, ivec3'.
--------------------------------------------------------------------------
Error-9: Variable assignment for 'floata' at Line 39:9 to Line 39:36, has expression of unknown type at Line 39:18 to Line 39:35 due to previous error(s).
     39:                  floata = dp3(ivec4a, inta);
                          ^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-10: Unmatched function parameters when calling function 'dp3' at Line 40:18 to Line 40:29.
     40:                  floata = dp3(ivec3a);
                                   ^^^^^^^^^^^ 

Info: Expecting function arguments 'vec4, vec4' or 'vec3, vec3' or 'ivec4, ivec4' or 'ivec3, ivec3'.
--------------------------------------------------------------------------
Error-11: Variable assignment for 'floata' at Line 40:9 to Line 40:30, has expression of unknown type at Line 40:18 to Line 40:29 due to previous error(s).
     40:                  floata = dp3(ivec3a);
                          ^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-12: Unmatched function parameters when calling function 'lit' at Line 41:17 to Line 41:27.
     41:                  vec4a = lit(vec3a);
                                  ^^^^^^^^^^ 

Info: Expecting function argument 'vec4'.
--------------------------------------------------------------------------
Error-13: Variable assignment for 'vec4a' at Line 41:9 to Line 41:28, has expression of unknown type at Line 41:17 to Line 41:27 due to previous error(s).
     41:                  vec4a = lit(vec3a);
                          ^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-14: Invalid variable assignment for 'vec3a' at Line 42:9 to Line 42:28, has expression of non-compatible type 'vec4' at Line 42:17 to Line 42:27.
     42:                  vec3a = lit(vec4a);
                          ^^^^^^^^^^^^^^^^^^^

Info: Variable 'vec3a' at Line 42:9 to Line 42:14 has type 'vec3', and is declared at Line 17:5 to Line 17:16:
     17:              vec3 vec3a;
                      ~~~~~~~~~~~
--------------------------------------------------------------------------
Warning-15: Read of potentially unassigned variable 'floata' of type 'float' at Line 25:18 to Line 25:24.
     25:              floata = rsq(floata);
                                   ^^^^^^  
--------------------------------------------------------------------------
Warning-16: Read of potentially unassigned variable 'inta' of type 'int' at Line 26:18 to Line 26:22.
     26:              floata = rsq(inta);
                                   ^^^^  
--------------------------------------------------------------------------
Warning-17: Read of potentially unassigned variable 'vec4a' of type 'vec4' at Line 27:18 to Line 27:23.
     27:              floata = dp3(vec4a, vec4a);
                                   ^^^^^         
--------------------------------------------------------------------------
Warning-18: Read of potentially unassigned variable 'vec4a' of type 'vec4' at Line 27:25 to Line 27:30.
     27:              floata = dp3(vec4a, vec4a);
                                          ^^^^^  
--------------------------------------------------------------------------
Warning-19: Read of potentially unassigned variable 'vec3a' of type 'vec3' at Line 28:18 to Line 28:23.
     28:              floata = dp3(vec3a, vec3a);
                                   ^^^^^         
--------------------------------------------------------------------------
Warning-20: Read of potentially unassigned variable 'vec3a' of type 'vec3' at Line 28:25 to Line 28:30.
     28:              floata = dp3(vec3a, vec3a);
                                          ^^^^^  
--------------------------------------------------------------------------
Warning-21: Read of potentially unassigned variable 'ivec4a' of type 'ivec4' at Line 29:18 to Line 29:24.
     29:              floata = dp3(ivec4a, ivec4a);
                                   ^^^^^^          
--------------------------------------------------------------------------
Warning-22: Read of potentially unassigned variable 'ivec4a' of type 'ivec4' at Line 29:26 to Line 29:32.
     29:              floata = dp3(ivec4a, ivec4a);
                                           ^^^^^^  
--------------------------------------------------------------------------
Warning-23: Read of potentially unassigned variable 'ivec3a' of type 'ivec3' at Line 30:18 to Line 30:24.
     30:              floata = dp3(ivec3a, ivec3a);
                                   ^^^^^^          
--------------------------------------------------------------------------
Warning-24: Read of potentially unassigned variable 'ivec3a' of type 'ivec3' at Line 30:26 to Line 30:32.
     30:              floata = dp3(ivec3a, ivec3a);
                                           ^^^^^^  
--------------------------------------------------------------------------
Warning-25: Read of potentially unassigned variable 'vec4b' of type 'vec4' at Line 31:17 to Line 31:22.
     31:              vec4a = lit(vec4b);
                                  ^^^^^  
--------------------------------------------------------------------------
Warning-26: Read of potentially unassigned variable 'ivec4a' of type 'ivec4' at Line 37:29 to Line 37:35.
     37:                  floata = dp3(vec4a, ivec4a);
                                              ^^^^^^  
--------------------------------------------------------------------------
Warning-27: Read of potentially unassigned variable 'vec3a' of type 'vec3' at Line 38:29 to Line 38:34.
     38:                  floata = dp3(vec4a, vec3a);
                                              ^^^^^  
--------------------------------------------------------------------------
Warning-28: Read of potentially unassigned variable 'ivec4a' of type 'ivec4' at Line 39:22 to Line 39:28.
     39:                  floata = dp3(ivec4a, inta);
                                       ^^^^^^        
--------------------------------------------------------------------------
Warning-29: Read of potentially unassigned variable 'inta' of type 'int' at Line 39:30 to Line 39:34.
     39:                  floata = dp3(ivec4a, inta);
                                               ^^^^  
--------------------------------------------------------------------------
Warning-30: Read of potentially unassigned variable 'ivec3a' of type 'ivec3' at Line 40:22 to Line 40:28.
     40:                  floata = dp3(ivec3a);
                                       ^^^^^^  
--------------------------------------------------------------------------
Warning-31: Read of potentially unassigned variable 'vec3a' of type 'vec3' at Line 41:21 to Line 41:26.
     41:                  vec4a = lit(vec3a);
                                      ^^^^^  
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
        (DECLARATION inta int)
        (DECLARATION vec4a vec4)
        (DECLARATION vec4b vec4)
        (DECLARATION vec3a vec3)
        (DECLARATION vec3b vec3)
        (DECLARATION ivec3a ivec3)
        (DECLARATION ivec3b ivec3)
        (DECLARATION ivec4a ivec4)
        (DECLARATION ivec4b ivec4)
    )
    (STATEMENTS
        (ASSIGN float floata (CALL rsq floata))
        (ASSIGN float floata (CALL rsq inta))
        (ASSIGN float floata (CALL dp3 vec4a vec4a))
        (ASSIGN float floata (CALL dp3 vec3a vec3a))
        (ASSIGN float floata (CALL dp3 ivec4a ivec4a))
        (ASSIGN float floata (CALL dp3 ivec3a ivec3a))
        (ASSIGN vec4 vec4a (CALL lit vec4b))
        (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN ANY_TYPE floata (CALL rsq floata floata))
                (ASSIGN ANY_TYPE floata (CALL rsq vec4a))
                (ASSIGN ANY_TYPE floata (CALL dp3 vec4a ivec4a))
                (ASSIGN ANY_TYPE floata (CALL dp3 vec4a vec3a))
                (ASSIGN ANY_TYPE floata (CALL dp3 ivec4a inta))
                (ASSIGN ANY_TYPE floata (CALL dp3 ivec3a))
                (ASSIGN ANY_TYPE vec4a (CALL lit vec3a))
                (ASSIGN ANY_TYPE vec3a (CALL lit vec4a))
            )
        )
    )
)
Failed to compile
