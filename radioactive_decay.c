#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
//#include <string.h>

//int main(int argc, char* argv[]) {
int main(void) {
  long double half_life = 388781648.64; // seconds
  long double sample_qt = 1.23e-6; // moles
  long double h = 1e-24; // moles --- quantity of matter where we can reasonably say there is virtually nothing because 1e-24 moles is less than 1 atom

  int t = 0;
  while(sample_qt > h) {
    printf("t_1/2 = %d\n", t);
    printf("n = %.Lg\n", sample_qt);
    sample_qt = sample_qt / 2;
    t++;
  }

  printf("the sample will be gone after %d half-lives\n", t);
  printf("so the sample will be gone in %Lg seconds", half_life * t);
  return 0;
}
