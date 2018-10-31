{
    const int a = 12;
    int b = 4 - 6 * 2;
    int c;
    vec3 v3;

    v3 = vec3(1., 2., 3.);
    a = (a + b);
    v3[1] = 5.5;

    if(a == 2) {
        int c;
        b = 2;
    } else
        b = 3;

    {
        int l1;
        {
            int l2;
            {
                int l3;
                {
                    int l4;
                }
            }
        }
    }
}