declarations -> epsilon
statements -> epsilon
variable -> identifier
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
expression -> expression - expression
expression -> integer_literal
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
expression -> integer_literal
expression -> expression - expression
expression -> (expression)
expression -> expression * expression
expression -> expression + expression
expression -> (expression)
expression -> expression + expression
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
program -> scope
