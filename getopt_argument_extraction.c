#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
//#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
//#include <string.h>

void print_usage(char* argv0) {
  fprintf(stderr, "usage: %s [-m mode] [-pv] dir [dir2] [dir3] ...\n", argv0);
}

int main(int argc, char* argv[]) {
  int opt;
  char* mode;
  char* program = argv[0];

  while((opt = getopt(argc, argv, ":m:pv")) != -1) {
    switch(opt) {
      case 'v':
        break;
      case 'p':
        break;
      case 'm':
        mode = optarg;
        break;
      case '?': // fallthrough
        fprintf(stderr, "%s: option error: unknown option \"-%c\"\n", program, optopt);
        print_usage(program);
        return 1;
        break;
      case ':':
        fprintf(stderr, "%s: option \"-%c\" needs an argument\n", program, optopt);
        print_usage(program);
        return 1;
        break;
    }
  }

  /*
  printf("argc=%d optind=%d *argv[optind]=%c\n", argc, optind, *argv[optind]);
  for(int i = 1; i < argc; i++) {
    if(*argv[i] != '-')
      printf("%s\n", argv[i]);
  }
  */

  if(optind == argc) {
    print_usage(program);
    return 1;
  }
  optind--;

  argv += optind;
  if(!argv[0]) { // no non-option argument is provided
    print_usage(program);
    return 1;
  }

  while(*++argv) {
    printf("%s\n", *argv); // Do stuff with args here
  }
  return 0;
}
