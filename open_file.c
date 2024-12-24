#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
#include <unistd.h>
#include <string.h>

// ADD TO CONFIG.H
#define STDIN_MAX 4096

void getline_stdin(char* buf) {
  fgets(buf, STDIN_MAX, stdin);
}

ssize_t get_filesize(FILE* fileptr) {
  if(fileptr == stdin)
    return -1;
  ssize_t filesize;
  if(fseek(fileptr, 0L, SEEK_END) != 0)
    return -1;
  if((filesize = ftell(fileptr)) == -1)
    return -1;
  if(fseek(fileptr, 0L, SEEK_SET) != 0)
    return -1;
  return filesize;
}

int readfile2buffer(FILE* fileptr, char* buf) {
  ssize_t filesize = get_filesize(fileptr);
  if(filesize == -1)
    return -1;

  size_t bytes_read = fread(buf, sizeof(char), filesize, fileptr);
  if(ferror(fileptr) != 0) {
    return -1;
  }

  buf[bytes_read++] = '\0'; // terminate buffer string
  return 0;
}

int main(int argc, char* argv[]) {
  int stdin_mode = 0;
  if(argc <= 1)
    stdin_mode = 1;
  
  char* buf;
  if(stdin_mode) {
    // TODO allow multiline stdin
    buf = malloc(STDIN_MAX);
    getline_stdin(buf);
    printf("%s", buf);
  } else {
    FILE* fileptr = fopen(argv[1], "r");
    if(fileptr == NULL) {
      fprintf(stderr, "x --- file '%s' not found\n", argv[1]);
      return 1;
    }
    buf = malloc(get_filesize(fileptr));
    if(readfile2buffer(fileptr, buf) == -1) {
      fprintf(stderr, "x --- read error\n");
      return 2;
    }
    printf("%s", buf);
    fclose(fileptr);
  }
  free(buf);
  return 0;
}
