#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
#include <string.h>
#include <sodium.h>
#include <time.h>

/*
  Compile with:
  'cc -std=c99 random_test1.c -Wall -Wextra -Wpedantic -lsodium'
*/

long double my_random() {
  unsigned char buf[1];
  randombytes_buf(buf, sizeof(buf));
  long double n = (double)buf[0] / 255.0; // Normalize to 0-1 range
  return n;
}

size_t str_is_nan(char* str) {
  int state = 0;
  for(size_t i = 0; i < strlen(str); i++) {
    switch(str[i]) {
      case '-': // fallthrough
      case '0': // fallthrough
      case '1': // fallthrough
      case '2': // fallthrough
      case '3': // fallthrough
      case '4': // fallthrough
      case '5': // fallthrough
      case '6': // fallthrough
      case '7': // fallthrough
      case '8': // fallthrough
      case '9':
        break;
      default:
        state = 1;
    }
  }
  return state;
}

int main(int argc, char* argv[]) {
  if(sodium_init() != 0) {
    fprintf(stderr, "sodium_init() failed\n");
    return 2;
  }

  if(argc <= 1) {
    fprintf(stderr, "please provide the number of times to run the experiment as an argument.\n");
    fprintf(stderr, "  eg. '$ %s 1000'\n", argv[0]);
    return 1;
  }

  if(str_is_nan(argv[1]) == 1) {
    fprintf(stderr, "%s is not a valid positive integer", argv[1]);
    return 1;
  }

  time_t start_time = time(NULL);

  long double my_sum = 0;
  size_t n = (size_t)atol(argv[1]);
  for(size_t i = 0; i < n; i++) {
    my_sum += my_random();
  }

  time_t end_time = time(NULL);
  // time_t delta_t = end_time - start_time;
  double delta_t = difftime(end_time, start_time);
  long double average = my_sum / n;

  printf("AVERAGE OF MULTIPLE VALUES BETWEEN 0 AND 1 ---\n");
  printf("time taken: %lfs\n", delta_t);
  printf("random numbers made: %zu\n", n);
  printf("result: %Lf\n", average);
  printf("inverted result: %Lf\n", (1 / average));
  printf("-------------------------\n");
  return 0;
}
