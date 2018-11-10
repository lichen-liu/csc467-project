
--------------------------------------------------------------------------
Error-0: Invalid indexing of non-vector variable 'a' of type 'int' at Line 3:13 to Line 3:17.
      3:              int b = a[0];
                              ^^^^ 
--------------------------------------------------------------------------
Error-1: Variable declaration of 'int b' at Line 3:5 to Line 3:18, is initialized to an unknown type at Line 3:13 to Line 3:17 due to previous error(s).
      3:              int b = a[0];
                      ^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Invalid indexing of non-vector variable 'a' of type 'int' at Line 4:13 to Line 4:17.
      4:              int c = a[1];
                              ^^^^ 
--------------------------------------------------------------------------
Error-3: Variable declaration of 'int c' at Line 4:5 to Line 4:18, is initialized to an unknown type at Line 4:13 to Line 4:17 due to previous error(s).
      4:              int c = a[1];
                      ^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-4: Read of potentially unassigned variable 'a' of type 'int' at Line 3:13 to Line 3:14.
      3:              int b = a[0];
                              ^    
--------------------------------------------------------------------------
Warning-5: Read of potentially unassigned variable 'a' of type 'int' at Line 4:13 to Line 4:14.
      4:              int c = a[1];
                              ^    
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
        (DECLARATION a int)
        (DECLARATION b int (INDEX ANY_TYPE a 0))
        (DECLARATION c int (INDEX ANY_TYPE a 1))
    )
    (STATEMENTS
    )
)
Failed to compile
