
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 13:8 to Line 13:18 have non-compatible type.
     13:              if(inta + 2.0)
                         ^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same base type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-1: If-statement condition expression has unknown type at Line 13:8 to Line 13:18. Expecting type 'bool'.
     13:              if(inta + 2.0)
                         ^^^^^^^^^^ 
--------------------------------------------------------------------------
Error-2: If-statement condition expression has type 'float' at Line 16:8 to Line 16:14. Expecting type 'bool'.
     16:              if(floata)
                         ^^^^^^ 
--------------------------------------------------------------------------
Error-3: If-statement condition expression has type 'bvec2' at Line 19:8 to Line 19:14. Expecting type 'bool'.
     19:              if(bvec2a)
                         ^^^^^^ 
--------------------------------------------------------------------------
Warning-4: Read of potentially unassigned variable 'boola' of type 'bool' at Line 10:8 to Line 10:13.
     10:              if(boola)
                         ^^^^^ 
--------------------------------------------------------------------------
Warning-5: Read of potentially unassigned variable 'inta' of type 'int' at Line 13:8 to Line 13:12.
     13:              if(inta + 2.0)
                         ^^^^       
--------------------------------------------------------------------------
Warning-6: Read of potentially unassigned variable 'floata' of type 'float' at Line 16:8 to Line 16:14.
     16:              if(floata)
                         ^^^^^^ 
--------------------------------------------------------------------------
Warning-7: Read of potentially unassigned variable 'bvec2a' of type 'bvec2' at Line 19:8 to Line 19:14.
     19:              if(bvec2a)
                         ^^^^^^ 
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
        (DECLARATION boola bool)
        (DECLARATION inta int)
        (DECLARATION floata float)
        (DECLARATION bvec2a bvec2)
    )
    (STATEMENTS
        (IF true )
        (IF boola )
        (IF (BINARY ANY_TYPE + inta 2.000000) )
        (IF floata )
        (IF bvec2a )
    )
)
Failed to compile
