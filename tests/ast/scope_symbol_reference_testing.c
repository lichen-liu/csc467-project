{
    int a;
    int b = a;
    int c = a;
    {
        a = 5;
        {
            int d = a;
            {
                int e = d;

                a = b;
            }
        }
    }
}