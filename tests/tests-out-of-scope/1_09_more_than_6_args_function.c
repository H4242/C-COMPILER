int function_with_6_args(int a, int b, int c, int d, int e, int f,int g)
{
    return a + b + c + d + e + f + g;
}

int main()
{
    int c = function_with_6_args(5, 6, 7, 8, 9, 10,11);
    return c;
}