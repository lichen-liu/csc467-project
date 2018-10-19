declarations -> epsilon
statements -> epsilon
variable -> identifier
expression -> variable
expression -> true
expression -> expression == expression
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

PARSER ERROR, LINE 7: syntax error, unexpected RBRACE
