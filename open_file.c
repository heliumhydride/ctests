#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/*
    NOTES
        [1] If we were loading a file >1MB, it's better to allocate the buffer on the heap.
        [2] If we were loading in text mode it's better to null terminate the whole buffer in case we do string-related operations
*/

ssize_t get_filesize(FILE* fileptr)
{
    if(fileptr == stdin)  return -1;
    ssize_t filesize;
    if(fseek(fileptr, 0L, SEEK_END) != 0) return -1;
    if((filesize = ftell(fileptr)) == -1) return -1;
    if(fseek(fileptr, 0L, SEEK_SET) != 0) return -1;
    return filesize;
}

int main(int argc, char* argv[])
{
    const char* program = argv[0];
    if(argc < 2) return 1;
    const char* filename = argv[1];

    FILE* fileptr = fopen(filename, "rb");
    if(fileptr == NULL) {
        fprintf(stderr, "%s: '%s': could not open file\n", program, filename);
        return 1;
    }

    size_t filesize = get_filesize(fileptr);
    uint8_t buffer[filesize];
    // [1]
    // uint8_t* buffer = malloc(filesize)

    size_t bytes_read = fread(buffer, sizeof(*buffer), filesize, fileptr);
    fclose(fileptr);

    // [2]
    // buffer[bytes_read] = '\0';

    // example, simple cheap hexdump clone
    for(size_t i = 0; i < bytes_read; ++i)
        printf("%02x ", buffer[i]);

    // [1]
    // free(buffer);
    putchar('\n');
    return 0;
}
