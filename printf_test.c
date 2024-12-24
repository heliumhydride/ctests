#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>

//int main(int argc, char* argv[]) {
int main(void) {
  // note: %b is glibc only and so not universal, so lets not test for that
  // add brackets between percents for prettiness
  int n = -1;
  char ch = 'a';
  char* str = "Hello";
  float x = 0.00000022f;
  printf("printf test\n\n");
  printf("  int %%d:     %d\n", n);
  printf("  int %%u:     %u\n", n);
  printf("  int %%x:     %x\n", n);
  printf("  int %%016x:  %016x\n", n);
  printf("  int %%o:     %o\n", n);
  printf("  char %%c:    %c\n", ch);
  printf("  char* %%s:   %s\n", str);
  printf("  char* %%p:   %p\n", str);
  printf("  float %%f:   %f\n", x);
  printf("  float %%e:   %e\n", x);
  printf("  float %%g:   %g\n", x);
  printf("  float %%a:   %a\n", x);
  return 0;
}
