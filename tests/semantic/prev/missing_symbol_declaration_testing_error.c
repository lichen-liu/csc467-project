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
    )
    (STATEMENTS
        (ASSIGN int a 3)
        (ASSIGN ANY_TYPE a b)
    )
)

--------------------------------------------------------------------------
Error-0: Missing declaration for symbol'b' at Line 5:9 to Line 5:10.
      5:              a = b;
                          ^ 
--------------------------------------------------------------------------
Error-1: Variable assignment for 'a' at Line 5:5 to Line 5:11, has expression of unknown type at Line 5:9 to Line 5:10 due to previous error(s).
      5:              a = b;
                      ^^^^^^
--------------------------------------------------------------------------
