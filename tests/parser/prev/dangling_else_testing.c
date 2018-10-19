declarations -> epsilon
statements -> epsilon
expression -> true | false
expression -> true | false
variable -> identifier
expression -> integer_literal
statement -> variable = expression ;
variable -> identifier
variable -> identifier
expression -> variable
expression -> integer_literal
expression -> expression / expression
statement -> variable = expression ;
else_statement -> else statement
statement -> if ( expression ) statement else_statement
else_statement -> epsilon
statement -> if ( expression ) statement else_statement
statements -> statements statement
scope -> { declarations statements }
program -> scope
