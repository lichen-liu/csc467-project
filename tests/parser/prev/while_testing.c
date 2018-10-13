declarations -> epsilon
statements -> epsilon
unary_op -> -
variable -> identifier[integer_literal]
expression -> variable
expression -> unary_op expression
binary_op -> ==
expression -> integer_literal
expression -> expression binary_op expression
declarations -> epsilon
statements -> epsilon
unary_op -> !
expression -> true | false
expression -> unary_op expression
statement -> ;
statement -> while ( expression ) statement
statements -> statements statement
scope -> { declarations statements }
statement -> scope
statement -> while ( expression ) statement
statements -> statements statement
scope -> { declarations statements }
program -> scope
