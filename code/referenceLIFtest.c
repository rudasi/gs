//An implementation of our standard LIF model. Run an example simulation
//like this:
//
//drive = -1:.002:1; %ramp input
//n = 50; # of neurons
//tauRef = .002; tauRC = .02;
//intercepts = -1+2*rand(1,n);
//maxRates = 100+100*rand(1,n);
//scales = ((1 ./ (1 - exp( (tauRef - (1 ./ maxRates)) / tauRC))) - 1) ./ (1 - intercepts);
//biases = 1 - scales .* intercepts;
//spikes = referenceLIF(.001, drive, .002, .02, scales, biases);
//imagesc(spikes), colormap('gray'), xlabel('Time (ms)'), ylabel('Neuron #')

//dt: time step (s)
//drive: input in the range [-1,1] (1 X #steps)
//tauRef: spike refractory period
//tauRC: membrane time constant
//scales: list of input scale factors for different neurons (1 X #neurons)
//biases: list of additive input biases for different neurons (1 X #neurons)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//Size of arrays
#define SIZE 50
#define ARRAY_SIZE 1024

void readCSVFile(char* filename, double* data){
   char str[ARRAY_SIZE];
   char* tok;
   int counter;
   FILE* stream = fopen(filename, "r");
   if (stream == NULL){
      printf("Unable to open file: %s", filename);
   } else {
     if (fgets(str, ARRAY_SIZE, stream) != NULL) {
        counter = 0;
        tok = strtok(str,",");
        while (tok != NULL){
           sscanf(tok, "%lf", &data[counter]);
           printf("%d: %lf\n",counter+1,data[counter]);
           counter++;
           tok = strtok(NULL,",");
        } 
     }
     else {
        printf("Unable to read characters from file: %s",filename);
     }
   }      
}

void referenceLIF(float dt, float drive[], float tauRef, float tauRC, float scales[], float biases[]);

void rand_gen(int count, float rand_num[]);

//float myround(float x, int places);

int main()
{
   int i;
   float dt = 0.001, n = 50, tauRef = 0.002, tauRC = 0.02;
   float scales[SIZE], biases[SIZE];
   double intercepts[SIZE], maxRates[SIZE];
   float drive[1001];
   //struct timeval t;
   float temp;
   //float rand_num[50] = {0};
   char file1[] = "intercepts.csv";
   char file2[] = "maxrates.csv";
   readCSVFile(file1,intercepts);
   readCSVFile(file2,maxRates);


   //rand_gen(50, rand_num);
/*
   FILE* rand_file;
   rand_file = fopen("rand_number.csv","a+");
   for(i=0;i<50;i++)
   {
      //myround(rand_num[i],4);
      fprintf(rand_file,",%f",rand_num[i]);
   }
   fclose(rand_file);
*/
   for(i=0;i<1001;i++)
   {
      drive[i] = -1.0 + (i * 0.002);
      //drive[i] = myround(drive[i],4);
   }
/*
   for(i=0;i<n;i++)
   {
   //   gettimeofday(&t,NULL);
   //   srand(t.tv_sec * t.tv_usec);
   //   printf("%f\n",rand_num[i]);
      intercepts[i] = -1 + 2*rand_num[i];
      //intercepts[i] = myround(intercepts[i],4);
   }


   for(i=0;i<n;i++)
   {
     // gettimeofday(&t,NULL);
     // srand(t.tv_sec * t.tv_usec);
      maxRates[i] = 100 + 100*rand_num[i+50];
      //maxRates[i] = myround(maxRates[i],4);
      printf("%f ",maxRates[i]);
   }
*/
   for(i=0;i<n;i++)
   {
      temp =  exp((tauRef - (1.0/maxRates[i])) / tauRC);
      //temp = myround(temp,4);
      scales[i] = ((1.0 / (1 - temp)) - 1) / (1 - intercepts[i]);
      //scales[i] = myround(scales[i],4);
      biases[i] = 1 - (scales[i] * intercepts[i]);
      //biases[i] = myround(biases[i],4);
   }

   referenceLIF(dt, drive, tauRef, tauRC, scales, biases);

return 0;
}

void referenceLIF(float dt, float drive[], float tauRef, float tauRC, float scales[], float biases[])
{
   int spikeIndices[SIZE] = {0};
   int i, j, counter, lastSpike[SIZE], spikes[SIZE][1001];
   float endTime, intTimes[SIZE], current[SIZE], dV[SIZE], V[SIZE] = {0};
   FILE* file;
   file = fopen("referenceLIF.csv","a+");

   for(i=0;i<50;i++)
   {
      lastSpike[i] = -1;
      for(j=0;j<1001;j++)
      {
         spikes[i][j] = 0;
      }
   }

   for(i=0;i<1001;i++)
   {
      endTime = dt*(i + 1);
      //endTime = myround(endTime, 4);
      counter = 0;
      for(j=0;j<50;j++)
      {
         //hack to reset spikeIndices instead of adding a new loop
         spikeIndices[j] = 0;

         intTimes[j] = fmin(dt, fmax(0.0, (endTime - lastSpike[j] - tauRef)));
         //intTimes[j] = myround(intTimes[j],4);
         current[j] = biases[j] + (scales[j] * drive[i]);
         //current[j] = myround(current[j],4);
         dV[j] = (current[j] - V[j]) / tauRC;
         //dV[j] = myround(dV[j],4);
         V[j] = V[j] + intTimes[j] * dV[j];
         //V[j] = myround(V[j],4);
      }

      for(j=0;j<50;j++)
      {
         V[j] = fmax(0.0,V[j]);
         if (V[j] >= 1.0)
         {
            spikeIndices[counter] = j;
            counter++;
         }
      }

      for(j=0;j<counter;j++)
      {
         lastSpike[spikeIndices[j]] = endTime - ((V[spikeIndices[j]] - 1) / dV[spikeIndices[j]]);
         V[spikeIndices[j]] = 0;
         spikes[spikeIndices[j]][i] = 1;
      }
   }

   fprintf(file,"%d",spikes[0][0]);
   for(i=0;i<50;i++)
   {
      for(j=1;j<1001;j++)
      {
         fprintf(file,",%d",spikes[i][j]);
      }
      fprintf(file,"\n");
   }

   fclose(file);
  //Open CV image plot
}

void rand_gen(int count, float rand_num[])
{
   int taps[4] = {32,22,2,1};
   int xi = 2135;
   int i, j, fb, temp;
   int mask = 1;
   unsigned int mod = ~0;
   for(i = 0; i < count; i++)
   {
      mask = mask << taps[0] - 1;
      fb = (xi & mask) >> (taps[0] - 1);
      for(j = 1; j < 4; j++)
      {
         mask = 1;
         mask = mask << taps[j] - 1;
         temp = (xi & mask) >> (taps[j] - 1);
         fb = fb ^ temp;
      }

      xi = xi << 1;
      if(fb > 0)
      {
         xi = xi | 1;
      }
      else
      {
         mask = ~0 << 1;
         xi = xi & mask;
      }
      rand_num[i] = fabs((xi*1.0)/mod);
     // rand_num[i] = myround(rand_num[i], 4);
      //fprintf(rand_file,",%.4f",rand_num[i]);
   }
}

/*
float myround(float x, int places)
{
   float const shift = powf(10.0f, places);
   x *= shift;
   x = roundf(x + 0.5f);
   x /= shift;

   return x;
}
*/







