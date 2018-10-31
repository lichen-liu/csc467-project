{
    int a;
    int b = a;
    const int c = 5;
    {
        a = 5;
        {
            int d = a;
            {
                int e = d;

                a = c;
            }
        }
    }
}