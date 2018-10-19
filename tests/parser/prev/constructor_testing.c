declarations -> epsilon
type -> ivec4
type -> ivec4
expression -> integer_literal
arguments -> expression
expression -> integer_literal
arguments -> arguments , expression
expression -> integer_literal
arguments -> arguments , expression
expression -> integer_literal
arguments -> arguments , expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> bool
type -> bool
expression -> true
arguments -> expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> vec2
type -> bvec2
expression -> integer_literal
arguments -> expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
