{
    /* Declaration */
    bool boola;
    bool boolb;
    bvec3 bvec3a = bvec3(true, false, true);
    bvec3 bvec3b;

    /* float */
    boola = !boolb;
    boola = !false;
    boola = -true;

    /* ivec3 */
    bvec3a = !bvec3b;
    bvec3a = -bvec3a;
    bvec3a = -bvec3(true, false, true);
    bvec3a = !bvec3(true, false, true);
}