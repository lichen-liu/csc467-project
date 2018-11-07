{
    /* Declaration */
    int inta = int(1);
    float floata = float(2.);
    bool boola = bool(true);
    ivec2 ivec2a = ivec2(1,inta);
    bvec3 bvec3a = bvec3(true, false, boola);
    vec4 vec4a = vec4(1., 2., floata, floata);

    /* invalid constructor */
    inta = int(1,2);
    inta = int(2.);
    ivec2a = ivec2(1, 2, 3);
    ivec2a = ivec2(1, 2.);
    ivec2a = ivec2(1);
    ivec2a = ivec2(true, 1);
}