
--------------------------------------------------------------------------
Error-0: Invalid variable assignment for Write-Only Result variable 'gl_FragDepth' in the scope of an if or else statement at Line 6:9 to Line 6:30.
      6:                  gl_FragDepth = false;
                          ^^^^^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'result bool gl_FragDepth'.
      6:                  gl_FragDepth = false;
                          ~~~~~~~~~~~~         
--------------------------------------------------------------------------
Error-1: Invalid variable assignment for Write-Only Result variable 'gl_FragDepth' in the scope of an if or else statement at Line 8:13 to Line 8:33.
      8:                      gl_FragDepth = true;
                              ^^^^^^^^^^^^^^^^^^^^

Info: Predefined Variable: 'result bool gl_FragDepth'.
      8:                      gl_FragDepth = true;
                              ~~~~~~~~~~~~        
--------------------------------------------------------------------------
Warning-2: Read of potentially unassigned variable 'vec4a' of type 'vec4' at Line 3:20 to Line 3:25.
      3:              gl_FragColor = vec4a;
                                     ^^^^^ 
--------------------------------------------------------------------------
Warning-3: Read of potentially unassigned variable 'vec4a' of type 'vec4' at Line 10:21 to Line 10:26.
     10:               gl_FragColor = vec4a;
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
        (DECLARATION vec4a vec4)
    )
    (STATEMENTS
        (ASSIGN vec4 gl_FragColor vec4a)
        (IF true (ASSIGN bool gl_FragDepth false))
        (IF false (ASSIGN bool gl_FragDepth true))
        (ASSIGN vec4 gl_FragColor vec4a)
    )
)
Failed to compile
