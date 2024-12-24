#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
#include <string.h>

void mingw_path(char* out, char* path) {
  size_t pathlen = strlen(path);
  for(size_t i = 0; i < pathlen; i++) {
    out[i] = path[i];
    if(path[i] == '\\')
      out[i] = '/';
  }
}

int main(int argc, char* argv[]) {
  if(argc <= 1)
    return 1;

  char* newpath = malloc(strlen(argv[1]));
  mingw_path(newpath, argv[1]);
  printf("%s\n", newpath);
  return 0;
}
