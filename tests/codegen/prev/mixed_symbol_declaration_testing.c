Info: Optimization for declaration of const-qualified symbol 'booltrue' of type 'const bool' successful at Line 2:5 to Line 2:32.
Info: Optimization for declaration of const-qualified symbol 'boolfalse' of type 'const bool' successful at Line 3:5 to Line 3:34.
Info: Optimization for declaration of const-qualified symbol 'ivec3a' of type 'const ivec3' successful at Line 4:5 to Line 4:41.
Info: Optimization for declaration of const-qualified symbol 'vec4a' of type 'const vec4' successful at Line 5:5 to Line 5:49.
!!ARBfp1.0

# User Declared Non-Constant Variables
TEMP   $inta_0                 ;
TEMP   $floata_0               ;
TEMP   $inta_1                 ;


# User Declared Constant Variables
PARAM  $floata_2               =  {0.0}                               ;
PARAM  $floata_1               =  state.light[0].half.w               ;
PARAM  $booltrue_0             =  1.000000                            ;
PARAM  $boolfalse_0            =  -1.000000                           ;
PARAM  $ivec3a_0               =  {1.000000,2.000000,3.000000}        ;
PARAM  $vec4a_0                =  {1.000000,2.000000,3.000000,4.000000};
PARAM  $vec4a_1                =  state.light[0].half                 ;


# Auto-Generated Re-usable Intermediate Value Registers


# Auto-Generated Non-reusable Intermediate Value Registers


# Auto-Generated Immediate Value Registers
PARAM  __$param_true           =  {1.0,1.0,1.0,1.0}                   ;
PARAM  __$param_false          =  {-1.0,-1.0,-1.0,-1.0}               ;
PARAM  __$param_zero           =  {0.0,0.0,0.0,0.0}                   ;


# Instructions

MOV    $floata_0               ,  __$param_zero           ;

MOV    $inta_0                 ,  __$param_zero           ;

MOV    $inta_1                 ,  __$param_zero           ;


END
