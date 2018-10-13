declarations -> epsilon
type -> int | ivec2 | ivec3 | ivec4
declaration -> type ID ;
declarations -> declarations declaration
statements -> epsilon
declarations -> epsilon
statements -> epsilon
variable -> identifier
expression -> integer_literal
statement -> variable = expression ;
statements -> statements statement
declarations -> epsilon
statements -> epsilon
variable -> identifier
expression -> integer_literal
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
statement -> scope
statements -> statements statement
scope -> { declarations statements }
statement -> scope
statements -> statements statement
scope -> { declarations statements }
program -> scope
