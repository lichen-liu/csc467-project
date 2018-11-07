
--------------------------------------------------------------------------
Error-0: Duplicate declaration of 'const int inta' at Line 4:5 to Line 4:24. Previously declared at Line 3:5 to Line 3:14.
      4:              const int inta = 2;
                      ^^^^^^^^^^^^^^^^^^^

Info: Previously declared here:
      3:              int inta;
                      ~~~~~~~~~
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'inta' of type 'const int' successful at Line 4:5 to Line 4:24.
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
        (DECLARATION inta const int 2)
    )
    (STATEMENTS
    )
)
Failed to compile
