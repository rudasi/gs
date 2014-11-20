#include <stdio.h>

typedef unsigned char uint8;

int main()
{
   int taps[4] = {32,22,2,1};
   int mask = 1;
   int temp = 0;
   int testval = 2135;

   mask = mask << taps[3] -1;
   temp = testval & mask;
   temp = temp >> taps[3] - 1;
   printf("%u\n",temp); 



 return 0;
}
