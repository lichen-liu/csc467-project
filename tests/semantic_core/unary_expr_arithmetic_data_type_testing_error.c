{
    /* Declaration */
    float floata;
    float floatb;
    ivec3 ivec3a = ivec3(10, 20, 30);
    ivec3 ivec3b;

    /* float */
    floata = -floatb;
    floata = -1.2;
    floata = !1.3;
    floata = !floatb;

    /* ivec3 */
    ivec3a = -ivec3b;
    ivec3a = !ivec3b;
    ivec3a = -ivec3(10, 20, -30);
    ivec3a = !ivec3(10, 20, -30);
}