#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 1000

typedef unsigned char u_int8;
typedef unsigned short u_int16;

int min(u_int16 a, u_int16 b)
{
   return (a < b) ? a:b;
}

int main()
{
   int i;
   int steps = SIZE;
   u_int16 x[SIZE] = {0};
   int input[SIZE] = {0};
   float y[SIZE] = {0};
   int alpha = 255;
   u_int16 bar;
   int decrement = 0;
   u_int8 r;
   struct timeval t;
   FILE* file;
   file = fopen("file_model1.csv","a+");


   for(i = 0; i < 10; i++)
   {
      input[(i*100)] = 150;
   }
   
   x[0] = input[0];
   y[0] = input[0];
 
   fprintf(file, "%hu,%f",x[0],y[0]); 
   
   for(i = 1; i < SIZE; i++)
   {
      bar = (u_int16)x[i-1] * (u_int16)256 - (u_int16)x[i-1] * (u_int16)alpha;
      decrement = bar >> 8;
      gettimeofday(&t, NULL);
      srand(t.tv_sec * t.tv_usec);
      r = (u_int8)(255 * (rand()/RAND_MAX));

      if(r < (bar % 256))
      {
         decrement = decrement + 1;
      }
 
      x[i] = min((u_int16)(255), (u_int16)(x[i-1] - decrement + input[i])); 
      y[i] = y[i-1] * (alpha/256.0) + input[i];
      fprintf(file, ",%hu,%f",x[i],y[i]); 
   }
               
 return 0;
}  
