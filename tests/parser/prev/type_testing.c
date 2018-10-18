declarations -> epsilon
type -> int | ivec2 | ivec3 | ivec4
declaration -> type ID ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
declaration -> type ID ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
declaration -> type ID ;
declarations -> declarations declaration
type -> int | ivec2 | ivec3 | ivec4
declaration -> type ID ;
declarations -> declarations declaration
type -> bool | bvec2 | bvec3 | bvec4
declaration -> type ID ;
declarations -> declarations declaration
type -> bool | bvec2 | bvec3 | bvec4
declaration -> type ID ;
declarations -> declarations declaration
type -> bool | bvec2 | bvec3 | bvec4
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
type -> float | vec2 | vec3 | vec4
declaration -> type ID ;
declarations -> declarations declaration
type -> float | vec2 | vec3 | vec4
declaration -> type ID ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> scope
