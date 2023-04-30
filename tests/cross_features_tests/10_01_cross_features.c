int power(int x, int n)
{
    if (n == 0)
    {
        return 1;
    }
    else
    {
        return x * power(x, n - 1);
    }
}

int main()
{
    int a = 5;
    int b = 1;
    while (a > 0)
    {
        b = power(b, power((a & b) * b)) * a;
        a = a - 1;
    }

    return b;
}