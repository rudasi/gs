//Testing similarity of exponential decay vs linear decay with
//exponential input

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef char u_int8;

int main()
{
   float dt = 0.001; //time step (s)
   float T = 0.25; //length of run (s)
   float tau = 0.02; //filter time constant
   int i, peak = 255, size = (int)(T/dt);
   float time[size];
   float u[size];
   float x[size];
   float y[size];
   float z[size];
   float temp;
   struct timeval t;
   FILE* file1;
   FILE* file2;
   file1 = fopen("file_model3_x.csv","a+");
   file2 = fopen("file_model3_z.csv","a+");

   for(i = 0; i < size; i++)
   {
      time[i] = dt*i + dt;
      gettimeofday(&t, NULL);
      srand(t.tv_sec * t.tv_usec);
      u[i] = 0.06 + 0.05*((rand()%4-2)*((1.0*rand())/RAND_MAX));
      x[i] = 0.0;
      y[i] = 0.0;
      z[i] = 0.0;
   }

   fprintf(file1,"%f,%f",x[0],dt);
   for(i = 1; i < size; i++)
   {
      x[i] = x[i-1] - dt*x[i-1]/tau + u[i-1];
      if(x[i] >= 1.0)
      {
         x[i] = 0.0;
      }
      fprintf(file1,",%f,%f",x[i],(dt*i)+dt);
      
      temp = expf((dt/tau)*((float)y[i-1] - peak)) + u[i-1]/5.0;
      y[i] = (u_int8)(y[i-1] - 1 + (tau/dt/temp) * u[i-1]);
      if(y[i] < 0.0 || y[i] >= peak)
      {
         y[i] = 0.0;
      }
   }
   
   z[0] = expf(dt/tau * (float)(y[0]) - peak);
   fprintf(file2,"%f,%f",z[0],dt);

   for(i = 1; i < size; i++)
   {
      z[i] = expf(dt/tau * (float)(y[i]) - peak);
      fprintf(file2,",%f,%f",z[i],(dt*i)+dt);
   }

 return 0;
}
