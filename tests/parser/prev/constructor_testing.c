declarations -> epsilon
type -> int | ivec2 | ivec3 | ivec4
type -> int | ivec2 | ivec3 | ivec4
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
type -> bool | bvec2 | bvec3 | bvec4
type -> bool | bvec2 | bvec3 | bvec4
expression -> true | false
arguments -> expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> float | vec2 | vec3 | vec4
type -> bool | bvec2 | bvec3 | bvec4
expression -> integer_literal
arguments -> expression
constructor -> type ( arguments )
expression -> constructor
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
