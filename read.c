#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
  FILE* fp = stdin;
  if(argc > 1) {
    if(strcmp(argv[1],"-"))
      fp = fopen(argv[1], "rb");
  }
  
  if(fp == NULL) {
    fprintf(stderr, "%s: file not found", argv[1]);
  }

  int c;
  while((c = fgetc(fp)) != EOF) {
    printf("%c", c);
  }

  fclose(fp);
  return 0;
}
