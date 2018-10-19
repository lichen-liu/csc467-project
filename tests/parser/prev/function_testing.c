declarations -> epsilon
type -> vec3
type -> vec3
expression -> float_literal
arguments -> expression
expression -> float_literal
arguments -> arguments , expression
expression -> float_literal
arguments -> arguments , expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> vec3
type -> vec3
expression -> float_literal
arguments -> expression
expression -> float_literal
arguments -> arguments , expression
expression -> float_literal
arguments -> arguments , expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> float
function_name -> lit | dp3 | rsq
variable -> identifier
expression -> variable
arguments -> expression
variable -> identifier
expression -> variable
arguments -> arguments , expression
arguments_opt -> arguments
function -> function_name ( arguments_opt )
expression -> function
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
variable -> identifier
function_name -> lit | dp3 | rsq
arguments_opt -> epsilon
function -> function_name ( arguments_opt )
expression -> function
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
program -> scope
