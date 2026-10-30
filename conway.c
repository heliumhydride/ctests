
/*
    Conways game of life in C, for Unix

    COMPILE
        Build with
            cc -O2 -std=c99 -Wall -Wextra -Wpedantic -Werror conway.c

        Optionally pass -DNO_UTF8 and -DNO_COLOR

    NOTES REGARDING WINDOWS
        It doesn't work yet, but when it will:
        To use UTF-8 characters in the CMD, run:
            chcp 65001
            ^^^ Seems to work on Win10 but not 8.1 from my testing

        Also you'll need a console host that supports ANSI escape codes.
        So either use ANSICON to extend the system conhost, or switch to ConEmu, Windows Terminal, ...

    TODOS
        TODO BUG [5] slowing down simulation speed slows down everything else
        TODO FEATURE allow loading board from a WIDTHxHEIGHT .ppm image file
        TODO    [3] support ppms with maxval > 255

    made with ♡ by heliumhydride
*/

#define _XOPEN_SOURCE 500   // usleep()
#include <ctype.h>          // isspace()
#include <stdbool.h>        // bool
#include <stdint.h>         // uint8_t
#include <stdio.h>          // putchar(), printf(), fopen(), fread(), ...
#include <string.h>         // memset()
#include <unistd.h>         // usleep()

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>        // BOOL, DWORD, SetConsoleCtrlHandler()
#include <conio.h>          // _kbhit(), _getch()
#else // On Unix
#include <poll.h>           // poll()
#include <signal.h>         // signal() (unix)
#include <termios.h>        // tcgetattr(), tcsetattr()
#endif // _WIN32

#define INIT_FPS 8

#if defined(NO_COLOR)
    #define ANSI_BLUE  ""
    #define ANSI_RED   ""
    #define ANSI_NORM  ""
#else
    #define ANSI_BLUE  "\033[1;34m"
    #define ANSI_RED   "\033[1;31m"
    #define ANSI_NORM  "\033[0m"
#endif // NO_COLOR

#define MATRIX_COORD(x,y) ((y)*WIDTH + (x))

#ifdef _WIN32
// TODO what if we plan to catch arrow keys later on ?
#define getch_buf(buf) (buf)[0] = _getch()
#else // On Unix
#define getch_buf(buf) read(STDIN_FILENO, (buf), sizeof((buf)))
#endif // _WIN32

#ifdef _WIN32
#define pollkey() _kbhit()
#else
#define pollkey() poll(fds, 1, 0)
#endif // _WIN32

#if defined(NO_UTF8)
    #define UPPER_HALF_BLOCK   "^"
    #define LOWER_HALF_BLOCK   "_"
    #define FULL_BLOCK         "@"
    #define VERTICAL_BAR       "|"
    #define HORIZONTAL_BAR     "-"
    #define LOWER_LEFT_CORNER  "\\"
    #define LOWER_RIGHT_CORNER "/"
    #define UPPER_LEFT_CORNER  "/"
    #define UPPER_RIGHT_CORNER "\\"
#else
    #define UPPER_HALF_BLOCK   "\xe2\x96\x80"
    #define LOWER_HALF_BLOCK   "\xe2\x96\x84"
    #define FULL_BLOCK         "\xe2\x96\x88"
    #define VERTICAL_BAR       "\xe2\x94\x82"
    #define HORIZONTAL_BAR     "\xe2\x94\x80"
    #define LOWER_LEFT_CORNER  "\xe2\x94\x94"
    #define LOWER_RIGHT_CORNER "\xe2\x94\x98"
    #define UPPER_LEFT_CORNER  "\xe2\x94\x8c"
    #define UPPER_RIGHT_CORNER "\xe2\x94\x90"
#endif // NO_UTF8

static bool running = true;
static size_t WIDTH = 32;
static size_t HEIGHT = 32;

/* Helper functions */

#ifdef _WIN32
BOOL ctrl_c(DWORD dwtype)
{
    if(dwtype == CTRL_C_EVENT) running = false;
    return true;
}
#else // On Unix
void ctrl_c(int unused)
{
    (void)unused;
    running = false;
}
#endif // _WIN32

void clear_screen(void)
{
    (void)printf("\033[H\033[J");
}

void print_upper_separator(void)
{
    printf(UPPER_LEFT_CORNER);
    for(size_t i = 0; i < WIDTH; ++i) printf(HORIZONTAL_BAR);
    printf(UPPER_RIGHT_CORNER "\n");
}

void print_lower_separator(void)
{
    printf(LOWER_LEFT_CORNER);
    for(size_t i = 0; i < WIDTH; ++i) printf(HORIZONTAL_BAR);
    printf(LOWER_RIGHT_CORNER "\n");
}

size_t count_population(const bool board[])
{
    size_t count = 0;
    for(size_t i = 0; i < (WIDTH*HEIGHT); ++i) {
        if(board[i]) ++count;
    }
    return count;
}

bool get_matrix_bc(const bool board[], const ssize_t x, const ssize_t y)    // get data from matrix, with bounds checking
{
    if(x < 0 || x >= (ssize_t)WIDTH)  return false;
    if(y < 0 || y >= (ssize_t)HEIGHT) return false;
    return board[MATRIX_COORD(x,y)];
}

int neighbours(const bool board[], const size_t x, const size_t y)
{
    int n = 0;
    n += (int)get_matrix_bc(board, x-1, y-1);
    n += (int)get_matrix_bc(board, x,   y-1);
    n += (int)get_matrix_bc(board, x+1, y-1);
    n += (int)get_matrix_bc(board, x-1, y);
    n += (int)get_matrix_bc(board, x+1, y);
    n += (int)get_matrix_bc(board, x-1, y+1);
    n += (int)get_matrix_bc(board, x,   y+1);
    n += (int)get_matrix_bc(board, x+1, y+1);
    return n;
}

/* Actual game code */

void update_board(bool board[])
{
    // from https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
    /*
        1. Any live cell with fewer than two live neighbours dies [...]
        (2. Any live cell with two or three live neighbours lives.)
        3. Any live cell with more than three live neighbours dies [...]
        4. Any dead cell with exactly three live neighbours becomes a live cell [...]
    */

    bool new_board[WIDTH*HEIGHT];
    memset(new_board, false, WIDTH*HEIGHT);

    for(size_t y = 0; y < HEIGHT; ++y) {
        for(size_t x = 0; x < WIDTH; ++x) {
            int n = neighbours(board, x, y);
            size_t idx = MATRIX_COORD(x, y);
            bool current = board[idx];
            
            if(current && (n == 2 || n == 3)) {
                new_board[idx] = true;
            } else if(!current && (n == 3)) {
                new_board[idx] = true;
            } else {
                new_board[idx] = false;
            }
        }
    }

    memcpy(board, new_board, sizeof(bool) * WIDTH * HEIGHT);
}

void display_board(const bool *board)
{
    for(size_t y = 0; y < HEIGHT; y+=2) {
        printf(VERTICAL_BAR);
        for(size_t x = 0; x < WIDTH; ++x) {
            uint8_t row_state = ((uint8_t)board[MATRIX_COORD(x,y)] << 1) | (uint8_t)board[MATRIX_COORD(x,y+1)]; // concatenate xth bool of top row with xth bool of bottom row
            switch(row_state) {
                case 0: // 0b00
                    putchar(' ');
                    break;
                case 1: // 0b01
                    printf(LOWER_HALF_BLOCK);
                    break;
                case 2: // 0b10
                    printf(UPPER_HALF_BLOCK);
                    break;
                case 3: // 0b11
                    printf(FULL_BLOCK);
                    break;
            }
        }
        printf(VERTICAL_BAR "\n");
    }
}

void print_usage(const char* program)
{
    (void)printf("usage: %s board.ppm\n", program);
    (void)printf("a default is provided in additional_files/conway/default.ppm\n");
}

int main(int argc, char* argv[])
{
    #ifdef _WIN32
    if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrl_c, true)) {
        fputs("unable to install handler.", stderr);
        return 1;
    }
    #else // On Unix
    signal(SIGINT, ctrl_c);
    signal(SIGTERM, ctrl_c);
    #endif // _WIN32

    const char* program = argv[0];
    if(argc < 2) {
        print_usage(program);
        return 1;
    }

    const char* filename = argv[1];
    FILE* fileptr = fopen(filename, "rb");
    if(fileptr == NULL) {
        fprintf(stderr, ANSI_RED "'%s': file not found\n" ANSI_NORM, filename);
        return 1;
    }
    
    char magic[3];
    fread(magic, sizeof(*magic), sizeof(magic), fileptr);
    if((magic[0] != 'P') || (magic[1] != '6') || !isspace(magic[2])) {
        fprintf(stderr, ANSI_RED "'%s': not a binary PPM formated image\n" ANSI_NORM, filename);
        return 1;
    }

    // skip comments because ppm images can have comments
    int c = getc(fileptr);
    while(c == '#') {
        while(getc(fileptr) != '\n');
        c = getc(fileptr);
    }
    ungetc(c, fileptr);

    size_t img_width, img_height;
    int color_maxval;
    fscanf(fileptr, "%zu %zu", &img_width, &img_height);
    fscanf(fileptr, "%d", &color_maxval);
    if(color_maxval > 255) {
        fprintf(stderr, "TODO  support maxval > 255\n");
        return 1;
    }
    (void)getc(fileptr); // skip one byte, trust me while debugging this aligned the pixel RGB data correctly (TODO needs more investigation)

    // allocating on the stack means an image <1MB, so 1000x1000 max
    uint8_t image_data[img_width * 3 * img_height];
    fread(image_data, img_width * 3, img_height, fileptr);
    fclose(fileptr);

    WIDTH  = img_width;
    HEIGHT = img_height;

    bool board[WIDTH*HEIGHT];
    memset(board, false, WIDTH*HEIGHT);

    // TODO [3]
    for(size_t i = 0; i < (img_width * img_height); ++i) {
        if((image_data[i*3] == color_maxval) && (image_data[i*3+1] == color_maxval) && (image_data[i*3+2] == color_maxval)) {
            board[i] = true;
        }
    }

    int fps = INIT_FPS;
    unsigned long generation = 1;

    #ifndef _WIN32 // On Unix

    /* the following code was stolen from myself (see 'cube.c') */
    struct termios old_tio, new_tio;
    tcgetattr(STDOUT_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDOUT_FILENO, TCSANOW, &new_tio);

    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    /* end of code stolen from self */

    #endif // !_WIN32

    running = true;
    while(running) {
        int polled = pollkey();
        if(polled == -1) {
            running = false;
        } else if(polled == 0) {
            clear_screen();
            printf("Controls:\n");
            printf("    SPACE   play/pause  "VERTICAL_BAR"  + - control speed\n");
            printf("    CTRL+C  exit        "VERTICAL_BAR"  * / control speed (5)\n");
            print_upper_separator();
            display_board(board);
            print_lower_separator();

            putchar('\n');
            printf(ANSI_BLUE "    GENERATION" ANSI_NORM "  " VERTICAL_BAR "  " ANSI_RED "%lu" ANSI_NORM "\n", generation);
            printf(ANSI_BLUE "    POPULATION" ANSI_NORM "  " VERTICAL_BAR "  " ANSI_RED "%zu" ANSI_NORM "\n", count_population(board));
            printf(ANSI_BLUE "         SPEED" ANSI_NORM "  " VERTICAL_BAR "  " ANSI_RED "%d FPS" ANSI_NORM "\n", fps);

            // TODO [5]
            usleep((useconds_t)(1000000/fps));
            update_board(board);
            ++generation;
        } else {
            char buf[3] = {0};
            getch_buf(buf);
            switch(buf[0]) {
                case '+':
                    ++fps;
                    break;
                case '-':
                    if(fps > 1) --fps;
                    break;
                case '*':
                    fps += 5;
                    break;
                case '/':
                    if((fps > 1) && (fps <= 5)) fps = 1;
                    if(fps > 5) fps -= 5;
                    break;
                case ' ':
                    printf(ANSI_RED "                   PAUSED\n" ANSI_NORM);
                    buf[0] = '\0';
                    while(buf[0] != ' ') {
                        getch_buf(buf);
                        usleep((useconds_t)1000);
                    }
                    break;
            }
        }
    }
    #ifndef _WIN32 // On Unix
    tcsetattr(STDOUT_FILENO, TCSANOW, &old_tio);
    #endif
    puts("\n            ok, goodbye.");
    return 0;
}
