
#define _POSIX_C_SOURCE 200112L

/// Useful includes
#include <stdio.h> // defines printf
//#include <stdlib.h>
//#include <getopt.h>
//#include <unistd.h>
//#include <string.h>

size_t strlen(const char* s) {
        const char* a = s;
        for(; *s; s++);
        return s-a;
}

int main(int argc, char* argv[]){
  if(argc > 1)
    printf("%zu\n", strlen(argv[1]));
  return 0;
}
