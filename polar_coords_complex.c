#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

// complier avec -DUSE_DEGREES=1 pour utiliser des degrés à la place des radians (les degrés seront convertis en radians)
#ifndef USE_DEGREES
#define USE_DEGREES 0
#endif

void print_usage(char* argv0) {
  fprintf(stderr, "usage: %s angle radius\n", argv0);
}

int main(int argc, char* argv[]) {
  if(argc<=2) {
    print_usage(argv[0]);
    return 1;
  }

  double th = atof(argv[1]);
  #if USE_DEGREES == 1
    th = th*M_PI/180;
  #endif
  double r = atof(argv[2]);

  // forule polaire d'un nombre complexe --> z = re^(iθ) ou z = r(cos(θ)+isin(θ))
  double complex z = r*cexp(th*I); // cexp() = exp() qui prend en charge des complexe
  printf("%.15f%+.15fi\n", creal(z), cimag(z));
  return 0;
}
