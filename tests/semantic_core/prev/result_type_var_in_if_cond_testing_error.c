
--------------------------------------------------------------------------
Error-0: If-statement condition expression has write-only Result type at Line 2:8 to Line 2:20.
      2:              if(gl_FragDepth)
                         ^^^^^^^^^^^^ 

Info: The first write-only Result variable is 'gl_FragDepth':
      2:              if(gl_FragDepth)
                         ~~~~~~~~~~~~ 
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
    )
    (STATEMENTS
        (IF gl_FragDepth )
    )
)
Failed to compile
