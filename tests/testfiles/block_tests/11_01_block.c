int main()
{
   int a = 1;
   int b = 1;
   {
      int z = 3;
      a = z;
   }
   return z;
}