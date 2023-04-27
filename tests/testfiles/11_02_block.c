int main()
{
   int a = 1;
   int b = 1;
   {
      int y = 0;
      {
         int z = 3;
         a = z + y;
      }
      z = 0;
   }
   return z;
}