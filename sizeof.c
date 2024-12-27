#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
#include <unistd.h>
//#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>

//int main(int argc, char* argv[]) {
int main(void) {
  printf("sizeof(void) = %zu\n", sizeof(void));
  printf("sizeof(bool) = %zu\n", sizeof(bool));
  printf("sizeof(char) = %zu\n", sizeof(char));
  printf("sizeof(unsigned char) = %zu\n", sizeof(unsigned char));
  printf("sizeof(short) = %zu\n", sizeof(short));
  printf("sizeof(unsigned short) = %zu\n", sizeof(unsigned short));
  printf("sizeof(int) = %zu\n", sizeof(int));
  printf("sizeof(unsigned int) = %zu\n", sizeof(unsigned int));
  printf("sizeof(long) = %zu\n", sizeof(long));
  printf("sizeof(unsigned long) = %zu\n", sizeof(unsigned long));
  printf("sizeof(long long) = %zu\n", sizeof(long long));
  printf("sizeof(unsigned long long) = %zu\n", sizeof(unsigned long long));
  printf("sizeof(ssize_t) = %zu\n", sizeof(ssize_t));
  printf("sizeof(size_t) = %zu\n", sizeof(size_t));
  printf("sizeof(float) = %zu\n", sizeof(float));
  printf("sizeof(double) = %zu\n", sizeof(double));
  printf("sizeof(long double) = %zu\n", sizeof(long double));
  printf("sizeof(int8_t) = %zu\n", sizeof(int8_t));
  printf("sizeof(uint8_t) = %zu\n", sizeof(uint8_t));
  printf("sizeof(int16_t) = %zu\n", sizeof(int16_t));
  printf("sizeof(uint16_t) = %zu\n", sizeof(uint16_t));
  printf("sizeof(int32_t) = %zu\n", sizeof(int32_t));
  printf("sizeof(uint32_t) = %zu\n", sizeof(uint32_t));
  printf("sizeof(int64_t) = %zu\n", sizeof(int64_t));
  printf("sizeof(uint64_t) = %zu\n", sizeof(uint64_t));
  printf("sizeof(intmax_t) = %zu\n", sizeof(intmax_t));
  printf("sizeof(uintmax_t) = %zu\n", sizeof(uintmax_t));
  printf("sizeof(float_t) = %zu\n", sizeof(float_t));
  printf("sizeof(double_t) = %zu\n", sizeof(double_t));
  return 0;
}
