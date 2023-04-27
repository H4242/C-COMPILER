int main()
{
    int i;

    while (i >= 0)
    {
        if (i % 2 == 0)
        {
            continue;
        }
        if (i > 5)
        {
            break;
        }
    }

    return 0;
}
