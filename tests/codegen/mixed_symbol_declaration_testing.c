{
    const bool booltrue = true;
    const bool boolfalse = false;
    const ivec3 ivec3a = ivec3(1, 2, 3);
    const vec4 vec4a = vec4(1.0, 2.0, 3.0, 4.0);
    float floata;
    {
        const vec4 vec4a = gl_Light_Half;
        const float floata = vec4a[3];
        int inta;
        {
            int inta;
            const float floata = gl_Light_Half[3] + 2.0;
        }
    }
}