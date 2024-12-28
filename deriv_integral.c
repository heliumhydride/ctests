#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
//#include <string.h>

double f(double x) {
  // x³-3x²+5x-2
  return sin(x);
}

double deriv(double(*f)(double), double x) {
  // (f(x+h)-f(x))/h
  double h = 1e-6;
  return (f(x+h) - f(x)) / h;
}

double integral(double (*f)(double), double a, double b, int n) {
  if(n % 2 != 0) {
    fprintf(stderr, "intervals must be even\n");
    return NAN;
  }

  double h = (b - a) / n;
  double int_res = f(a) + f(b);

  for (int i = 1; i < n; i++) {
    if (i % 2 == 0) {
      int_res += 2 * f(a + i * h);
    } else {
      int_res += 4 * f(a + i * h);
    }
  }

  return int_res * h / 3;
}

//int main(int argc, char* argv[]) {
int main(void) {
  printf("f(1) = %.lg\n", f(1));
  printf("f'(1) = %.lg\n", deriv(f,1));
  printf("integral(f(x), 0, pi) = %.lg\n", integral(f,0,M_PI,1000));
  return 0;
}
