#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>

#define PI atan(1.0)/4.0

bool IsPowerOfTwo(int n)
{
  return (n != 0) && ((n & (n - 1)) == 0);
}

void separate(double complex *a, int n)
{
  double complex *b = malloc(sizeof(double complex) * n / 2);
  register int i;

  for(i = 0; i < n/2; i++)
    b[i] = a[i*2+1];
  for(i = 0; i < n/2; i++)
    a[i] = a[i*2];
  for(int i = 0; i < n/2; i++)
    a[i+n/2] = b[i];
  free(b);
}

void fft(double complex *a, int n)
{
  register int k;
  
  if(n < 2){
    /* do nothing */
  } else if(IsPowerOfTwo(n)) {
    separate(a, n);
    fft(a, n/2);
    fft(a+n/2, n/2);

    for(k = 0; k < n/2; k++) {
      complex double e = a[k];
      complex double o = a[k+n/2];
      complex double w = cexp(-2.0*PI*k/n*I);
      a[k] = e + w * o;
      a[k+n/2] = e - w * o;
    }
  } else {
    /* do nothing */
  }
}

int main(int argc, char *argv[])
{
#define NUM 1024
  register int i;

  FILE *freal, *fimag;

  freal = fopen("./real.out", "w");

  if(!freal) {
    
    exit(EXIT_FAILURE);
  }
  
  fimag = fopen("./imag.out", "w");

  if(!fimag) {

    fclose(freal);
    exit(EXIT_FAILURE);
  }
  
  double complex *a = malloc(sizeof(double complex) * NUM);

  for(i = 0; i < NUM; i++)
    a[i] = sin(2.0*PI*i/NUM) + 3.0*sin(2.0*PI*4.0*i/NUM) + 3.5*cos(2.0*PI*3.7548*i/NUM);

  fft(a, NUM);

  for(i = 0; i < NUM; i++) {
    printf("%.2f %+.2fi\n", creal(a[i]), cimag(a[i]));
    fprintf(freal, "%i\t%.15f\n", i, creal(a[i]));
    fprintf(fimag, "%i\t%.15f\n", i, cimag(a[i]));
  }
  
  free(a);

  fclose(freal);
  fclose(fimag);
  
  return 0;
}
