declarations -> epsilon
statements -> epsilon
variable -> identifier
unary_op -> -
variable -> identifier
expression -> variable
expression -> unary_op expression
variable -> identifier
expression -> variable
expression -> expression - expression
expression -> integer_literal
expression -> expression - expression
unary_op -> !
expression -> integer_literal
expression -> unary_op expression
expression -> expression - expression
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
program -> scope
