#include <stdio.h>
#include <math.h>

#define PI 3.1415

// Check for built in C99 stuff instead of this struct
typedef struct ComplexNum   
{
    double real;
    double imag;
} ComplexNum; 

void dft(ComplexNum* in, ComplexNum* out, int insize, int outsize)
{
    int i;
    for (i = 0; i < outsize; i++)
    {
        ComplexNum sum;
        sum.real = 0;
        sum.imag = 0;
        
        int t;
        for (t = 0; t < insize; t++)
        {
            double angle = 2 * PI * t * i / insize; 
            sum.real += in[t].real * cos(angle) + in[t].imag * sin(angle); 
            sum.imag += -in[t].real * sin(angle) + in[t].imag * cos(angle);
        } 
        
        out[i].real = sum.real;
        out[i].imag = sum.imag;
    }
}

int main(int args, char** argv)
{
    ComplexNum input[] = {
    {.real = 1},
    {.real = 0},
    {.real = -1},
    {.real = 0}
    };     
   
    ComplexNum output[12];

    dft(input, output, 4, 12);
 
    int i;
    for (i = 0; i < 12; i++)
        printf("%.3lf\n", sqrt(output[i].real*output[i].real + output[i].imag*output[i].imag));

    return 0;
}
