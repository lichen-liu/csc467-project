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
    )
    (STATEMENTS
        (IF gl_FragDepth )
    )
)

--------------------------------------------------------------------------
Error-0: If-statement condition expression has write-only Result type at Line 2:8 to Line 2:20.
      2:              if(gl_FragDepth)
                         ^^^^^^^^^^^^ 

Info: The first write-only Result variable is 'gl_FragDepth':
      2:              if(gl_FragDepth)
                         ~~~~~~~~~~~~ 
--------------------------------------------------------------------------
