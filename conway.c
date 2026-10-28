
/*
    Conways game of life in C, for Unix

    COMPILE
        Build with
            cc -O2 -std=c99 -Wall -Wextra -Wpedantic -Werror conway.c

        Optionally pass -DNO_UTF8 and -DNO_COLOR

    TODOS
        TODO allow loading board from a WIDTHxHEIGHT .ppm image file
        TODO    maybe set width/height from loading the board ?
        TODO    supply a default board if none is provided ?
        TODO keysbinds
        TODO    allow changing speed by pressing +/- on kb (like cube.c)
        TODO    play/pause simulation on spacebar

    made with ♡ by heliumhydride
*/

#define _XOPEN_SOURCE 500   // usleep()
#include <stdbool.h>        // bool
#include <stdint.h>         // uint8_t
#include <stdio.h>          // putchar(), printf() et al
#include <string.h>         // memset()
#include <unistd.h>         // usleep()

#define WIDTH       32
#define HEIGHT      32
#define INIT_FPS    15

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

/* Helper functions */

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

size_t count_population(bool board[])
{
    size_t count = 0;
    for(size_t i = 0; i < (WIDTH*HEIGHT); ++i) {
        if(board[i]) ++count;
    }
    return count;
}

bool get_matrix_bc(bool board[], ssize_t x, ssize_t y)    // get data from matrix, with bounds checking
{
    if(x < 0 || x >= WIDTH)  return false;
    if(y < 0 || y >= HEIGHT) return false;
    return board[MATRIX_COORD(x,y)];
}

int neighbours(bool board[], size_t x, size_t y)
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

    bool new_board[WIDTH*HEIGHT] = {0};

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

void display_board(bool *board)
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

int main(void)
{
    bool board[WIDTH*HEIGHT] = {false};

    // initialize a blinker in the center of the display
    board[MATRIX_COORD(WIDTH/2,HEIGHT/2)] = true;
    board[MATRIX_COORD(WIDTH/2 - 1,HEIGHT/2)] = true;
    board[MATRIX_COORD(WIDTH/2 + 1,HEIGHT/2)] = true;

    // create a glider at the top left corner of the display 
    board[MATRIX_COORD(1,0)] = true;
    board[MATRIX_COORD(2,1)] = true;
    board[MATRIX_COORD(0,2)] = true;
    board[MATRIX_COORD(1,2)] = true;
    board[MATRIX_COORD(2,2)] = true;

    int fps = INIT_FPS;
    size_t generation = 1;

    clear_screen();
    while(true) {
        printf("Controls:\n");
        printf("    SPACE play/pause     +/- control speed      CTRL+C exit\n\n");
        print_upper_separator();
        display_board(board);
        print_lower_separator();

        putchar('\n');
        printf("    [" ANSI_BLUE "GENERATION" ANSI_NORM "]    " ANSI_RED "%zu" ANSI_NORM "\n", generation);
        printf("    [" ANSI_BLUE "POPULATION" ANSI_NORM "]    " ANSI_RED "%zu" ANSI_NORM "\n", count_population(board));
        printf("    [" ANSI_BLUE "     SPEED" ANSI_NORM "]    " ANSI_RED "%d FPS" ANSI_NORM "\n", fps);

        usleep((useconds_t)(1000000/fps));

        clear_screen();
        update_board(board);
        ++generation;
    }
    return 0;
}
