int main()
{
    int a = 2;
    if (a + 1 == 4)
    {
        int b = 1;
    }
    else
    {
        if (b == 1 + a)
        {
            a = 5;
        }
        else
        {
            a = 6;
        }
    }
    int c = a + b;
    return c;
}