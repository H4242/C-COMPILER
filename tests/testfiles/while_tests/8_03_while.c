int main()
{
   int a = 1;
   int b = 1;
   while (a < 3)
   {
      a = a + 1;
      if (b == 2)
      {
         a = a + 2;
      }
   }
   return a;
}
