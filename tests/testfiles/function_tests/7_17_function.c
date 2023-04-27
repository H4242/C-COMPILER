int function_with_6_args(int a, int b, int c, int d, int e, int f)
{
    return a + b + c + d + e + f;
}

int main()
{
    int c = function_with_6_args(5, 6, 7, 8, 9, 10);
    return c;
}