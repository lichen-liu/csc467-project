declarations -> epsilon
type -> int | ivec2 | ivec3 | ivec4
declaration -> type ID ;
declarations -> declarations declaration
type -> bool | bvec2 | bvec3 | bvec4
declaration -> type ID ;
declarations -> declarations declaration
type -> float | vec2 | vec3 | vec4
declaration -> type ID ;
declarations -> declarations declaration
type -> float | vec2 | vec3 | vec4
declaration -> type ID ;
declarations -> declarations declaration
type -> bool | bvec2 | bvec3 | bvec4
expression -> true | false
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
expression -> integer_literal
binary_op -> +
expression -> integer_literal
expression -> expression binary_op expression
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> float | vec2 | vec3 | vec4
expression -> float_literal
binary_op -> *
expression -> float_literal
expression -> expression binary_op expression
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
expression -> integer_literal
binary_op -> *
expression -> integer_literal
expression -> expression binary_op expression
declaration -> const type ID = expression ;
declarations -> declarations declaration
type -> float | vec2 | vec3 | vec4
expression -> float_literal
declaration -> const type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
