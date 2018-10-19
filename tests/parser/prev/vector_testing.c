declarations -> epsilon
type -> int | ivec2 | ivec3 | ivec4
type -> int | ivec2 | ivec3 | ivec4
expression -> integer_literal
arguments -> expression
expression -> integer_literal
arguments -> arguments , expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
variable -> identifier[integer_literal]
expression -> variable
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
variable -> identifier[integer_literal]
expression -> variable
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
variable -> identifier[integer_literal]
expression -> integer_literal
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
program -> scope
