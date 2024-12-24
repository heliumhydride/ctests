#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h>  // defines printf
#include <stdlib.h> // defines malloc
#include <stdint.h> // defines size_t, uint*_t, ...
#include <unistd.h> // defines ssize_t
//#include <getopt.h>
//#include <string.h>

char* itoa(const int n, char* str, const int base) {
  int digit = 0;
  ssize_t new_n = n;
  char* ptr = str; char* ptr1 = str;
  char* chars = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
  char ch;

  if(base > 36) {
    *ptr++ = '\0';
    return str;
  }

  // TODO printed in reverse
  // note: this makes it in reverse, we need to reverse everything afterwards
  while(new_n != 0) {
    digit = new_n % base;
    *ptr++ = chars[35+digit];
    new_n = (int)(new_n/base);
  }

  if(n < 0) {
    *ptr++ = '-';
  }

  *ptr-- = '\0'; // finish string
  while(ptr1 < ptr) { // reverse string
    ch = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = ch;
  }
  return str;
}

int main(int argc, char* argv[]){
  char* buffer = malloc(33);
  ssize_t intarr[] = {255,-12,12023,12345,43569,1111};

  for(int i = 0; i < sizeof(intarr)/sizeof(ssize_t); i++) {
    printf("n=%d\n", intarr[i]);
    itoa(intarr[i], buffer, 10); printf("  dec: %s\n", buffer);
    itoa(intarr[i], buffer, 16); printf("  hex: %s\n", buffer);
    itoa(intarr[i], buffer, 2);  printf("  bin: %s\n", buffer);
    itoa(intarr[i], buffer, 36); printf("  b36: %s\n", buffer);
    printf("\n");
  }
	return 0;
}
