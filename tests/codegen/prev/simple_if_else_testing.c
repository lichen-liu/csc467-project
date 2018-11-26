!!ARBfp1.0

# User Declared Non-Constant Variables
TEMP   $intb_1                 ;
TEMP   $intb_0                 ;
TEMP   $inta_0                 ;


# User Declared Constant Variables


# Auto-Generated Re-usable Intermediate Value Registers
TEMP   __$temp_0               ;
TEMP   __$temp_1               ;
TEMP   __$temp_2               ;


# Auto-Generated Non-reusable Intermediate Value Registers
TEMP   __$templl_0             ;


# Auto-Generated Immediate Value Registers
PARAM  __$param_true           =  {1.0,1.0,1.0,1.0}                   ;
PARAM  __$param_false          =  {-1.0,-1.0,-1.0,-1.0}               ;
PARAM  __$param_zero           =  {0.0,0.0,0.0,0.0}                   ;
PARAM  __$param_0              =  3.000000                            ;
PARAM  __$param_1              =  2.000000                            ;
PARAM  __$param_2              =  2.000000                            ;
PARAM  __$param_3              =  3.000000                            ;
PARAM  __$param_4              =  4.000000                            ;


# Instructions

MOV    $inta_0                 ,  __$param_0              ;

ADD    __$temp_0               ,  $inta_0                 ,  __$param_1              ;
MOV    $inta_0                 ,  __$temp_0               ;

# Evaluate if statement condition
SUB    __$temp_1               ,  __$param_2              ,  $inta_0                 ;
CMP    __$temp_0               ,  __$temp_1               ,  __$param_true           ,  __$param_false          ;
MOV    __$temp_2.x             ,  __$temp_0.x             ;
MOV    __$temp_2.y             ,  __$temp_0.x             ;
MOV    __$temp_2.z             ,  __$temp_0.x             ;
MOV    __$temp_2.w             ,  __$temp_0.x             ;
# Set the condition for outer-most if statement
MOV    __$templl_0             ,  __$temp_2               ;

CMP    $intb_0                 ,  __$templl_0             ,  $intb_0                 ,  __$param_3              ;

# Negate the condition for outer-most else statement
MOV    __$templl_0             ,  -__$templl_0            ;

CMP    $intb_1                 ,  __$templl_0             ,  $intb_1                 ,  __$param_4              ;


END
