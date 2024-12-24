#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
#include <string.h>

int is_printable_ch(char c) {
  /*
    if(c == '\n')
      return 1;
  */
  /*
    if(c == '\r')
      return 1;
  */
  if(c >= 32 && c <= 126) // From Space to ~
    return 1;
  // if checks fail return 0
  return 0;
}

//int main(int argc, char* argv[]) {
int main(void) {
  char* buffer = "hello\001chars ch[]()\033[0m";
  char ch;
  char* tmp_str = malloc(sizeof(*buffer) * strlen(buffer));
  char* ptr = tmp_str;

  for(size_t i = 0; i < strlen(buffer); i++) {
    ch = buffer[i];
    if(is_printable_ch(ch)) {
      *ptr++ = ch;
    }
    *ptr-- = '\0'; // terminate string
  }
  printf("%s\n", tmp_str);
  free(tmp_str);
  ptr = tmp_str; // reset string pointer to be able to repeat the loop
  return 0;
}
