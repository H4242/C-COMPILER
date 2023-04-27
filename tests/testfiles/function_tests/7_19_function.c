int fonc(int n)
{
    int a = 0;
    {
        int b = a;
        a = n;
        a = a + b;
    }
    return a;
}

int main()
{
    {
        int z = 0;
    }
    return fonc(10);
}