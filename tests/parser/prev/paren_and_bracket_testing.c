declarations -> epsilon
type -> float | vec2 | vec3 | vec4
function_name -> lit | dp3 | rsq
variable -> identifier[integer_literal]
expression -> variable
expression -> integer_literal
expression -> expression + expression
expression -> (expression)
expression -> integer_literal
expression -> expression * expression
arguments -> expression
variable -> identifier
expression -> variable
arguments -> arguments , expression
function_name -> lit | dp3 | rsq
variable -> identifier
expression -> variable
arguments -> expression
expression -> integer_literal
arguments -> arguments , expression
arguments_opt -> arguments
function -> function_name ( arguments_opt )
expression -> function
arguments -> arguments , expression
arguments_opt -> arguments
function -> function_name ( arguments_opt )
expression -> function
expression -> integer_literal
unary_op -> -
expression -> integer_literal
expression -> unary_op expression
function_name -> lit | dp3 | rsq
function_name -> lit | dp3 | rsq
function_name -> lit | dp3 | rsq
arguments_opt -> epsilon
function -> function_name ( arguments_opt )
expression -> function
arguments -> expression
arguments_opt -> arguments
function -> function_name ( arguments_opt )
expression -> function
arguments -> expression
arguments_opt -> arguments
function -> function_name ( arguments_opt )
expression -> function
expression -> expression * expression
expression -> expression + expression
expression -> (expression)
expression -> expression + expression
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
