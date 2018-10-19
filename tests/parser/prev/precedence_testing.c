declarations -> epsilon
type -> int
expression -> false
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
expression -> integer_literal
expression -> integer_literal
function_name -> lit | dp3 | rsq
arguments_opt -> epsilon
function -> function_name ( arguments_opt )
expression -> function
expression -> expression + expression
expression -> (expression)
unary_op -> -
expression -> integer_literal
expression -> unary_op expression
type -> int
variable -> identifier
expression -> variable
arguments -> expression
expression -> integer_literal
arguments -> arguments , expression
variable -> identifier
expression -> variable
arguments -> arguments , expression
constructor -> type ( arguments )
expression -> constructor
expression -> expression + expression
expression -> (expression)
expression -> expression ^ expression
expression -> expression ^ expression
expression -> expression / expression
expression -> expression - expression
expression -> (expression)
expression -> expression == expression
expression -> expression && expression
expression -> expression || expression
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
