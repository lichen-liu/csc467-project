declarations -> epsilon
type -> int
declaration -> type ID ;
declarations -> declarations declaration
type -> bvec2
declaration -> type ID ;
declarations -> declarations declaration
type -> float
declaration -> type ID ;
declarations -> declarations declaration
type -> vec4
declaration -> type ID ;
declarations -> declarations declaration
type -> bool
expression -> true
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int
expression -> integer_literal
expression -> integer_literal
expression -> expression + expression
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> float
expression -> float_literal
expression -> float_literal
expression -> expression * expression
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int
expression -> integer_literal
expression -> integer_literal
expression -> expression * expression
declaration -> const type ID = expression ;
declarations -> declarations declaration
type -> float
expression -> float_literal
declaration -> const type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
