declarations -> epsilon
statements -> epsilon
variable -> identifier
expression -> variable
binary_op -> ==
expression -> true | false
expression -> expression binary_op expression
declarations -> epsilon
statements -> epsilon
variable -> identifier
expression -> integer_literal
statement -> variable = expression ;
statements -> statements statement
unary_op -> !
variable -> identifier
expression -> variable
expression -> unary_op expression
variable -> identifier
expression -> integer_literal
statement -> variable = expression ;
else_statement -> epsilon
statement -> if ( expression ) statement else_statement
statements -> statements statement
scope -> { declarations statements }
statement -> scope
declarations -> epsilon
statements -> epsilon
variable -> identifier
expression -> integer_literal
binary_op -> *
expression -> true | false
expression -> expression binary_op expression
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
statement -> scope
else_statement -> else statement
statement -> if ( expression ) statement else_statement
statements -> statements statement
scope -> { declarations statements }
program -> scope
