{
    /*
     * +, - ss, vv Arithmetic
     */

    /* Declaration */
    float floata;
    float floatb;
    float floatc;
    vec3 vec3a;
    ivec3 ivec3a = ivec3(10, 20, 30);
    ivec3 ivec3b;
    ivec3 ivec3c;
    int inta;

    /* scalar */
    floata = floata + floatb;
    floata = 1.2 - floatb;
    floata = -1.2 + floatc;

    /* vec */
    ivec3a = ivec3c + ivec3b;
    ivec3a = ivec3b - ivec3(10, 20, -30);

    /* mix scalar and vec */
    vec3a = vec3a - floata;
    inta = ivec3c + inta;

    /* mix base type */
    floata = floata + inta;
}