int alphabet(int n)
{
    int a = 'A';
    while (a < 'A' + n)
    {
        putchar(a);
        a = a + 1;
    }
    return a;
}
int main()
{
    char r = alphabet(15);
    return r;
}
