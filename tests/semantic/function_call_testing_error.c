{
    /*
        float rsq(float);
        float rsq(int);
        float dp3(vec4, vec4);
        float dp3(vec3, vec3);
        float dp3(ivec4, ivec4);
        float dp3(ivec3, ivec3);
        vec4 lit(vec4);
    */

    /* Declaration */
    float floata;
    int inta;
    vec4 vec4a;
    vec4 vec4b;
    vec3 vec3a;
    vec3 vec3b;
    ivec3 ivec3a;
    ivec3 ivec3b;
    ivec4 ivec4a;
    ivec4 ivec4b;

    /* Function Calls */
    floata = rsq(floata);
    floata = rsq(inta);
    floata = dp3(vec4a, vec4a);
    floata = dp3(vec3a, vec3a);
    floata = dp3(ivec4a, ivec4a);
    floata = dp3(ivec3a, ivec3a);
    vec4a = lit(vec4b);

    {
        /* Invalid Function Calls */
        floata = rsq(floata, floata);
        floata = rsq(vec4a);
        floata = dp3(vec4a, ivec4a);
        floata = dp3(vec4a, vec3a);
        floata = dp3(ivec4a, inta);
        floata = dp3(ivec3a);
        vec4a = lit(vec3a);
        vec3a = lit(vec4a);
    }
}