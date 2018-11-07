{
    /*
     * +, - ss, vv Arithmetic
     */

    /* Declaration */
    float floata = 0.;
    float floatb = 1.;
    float floatc = 2.;
    vec3 vec3a = vec3(1., 2., 3.);
    ivec3 ivec3a = ivec3(10, 20, 30);
    ivec3 ivec3b = ivec3(1,2,3);
    ivec3 ivec3c = ivec3(4,5,6);
    ivec4 ivec4a = ivec4(5,6,7,8);
    int inta;

    /* scalar */
    floata = floata + floatb;
    floata = 1.2 - floatb;
    floata = -1.2 + floatc;

    /* vec */
    ivec3a = ivec3c + ivec3b;
    ivec3a = ivec3b - ivec3(10, 20, -30);
    ivec3a = ivec3a + ivec4a;

    /* mix scalar and vec */
    vec3a = vec3a - floata;
    inta = ivec3c + inta;

    /* mix base type */
    floata = floata + inta;
}