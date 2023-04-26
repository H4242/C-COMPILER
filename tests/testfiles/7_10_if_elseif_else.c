int main()
{
    int a = 2;
    int b = 1;
    if (a == 2)
    {
        if (b == 1)
        {
            a = 5;
        }
        else
        {
            a = 6;
        }
    }
    else
    {
        a = 7;
    }
    int c = a + b;
    return c;
}