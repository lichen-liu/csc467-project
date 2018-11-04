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
