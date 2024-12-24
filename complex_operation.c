#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <math.h>
#include <complex.h>

//int main(int argc, char* argv[]) {
int main(void) {
  double complex z1 = cpow(1.0f*I,1.0f/I);
  double complex z2 = cexp(M_PI/2);
  printf("iroot(i) = %.15f%+.15fi\n", creal(z1), cimag(z1));
  printf("e^pi/2 = %.15f%+.15fi\n", creal(z2), cimag(z2));
  return 0;
}
