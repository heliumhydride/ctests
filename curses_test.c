#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <curses.h>
//#include <menu.h>
//#include <panel.h>
#include <stdlib.h>
#include <string.h>
//#include <getopt.h>

static const int h_margin = 7; // horizontal character margin (==space between screen border and window border)
static const int v_margin = 5; // same but vertical
static const int v_padding = 1; // horizontal character padding (==space between window border and text)
static const int h_padding = 2; // same but vertical

//int main(int argc, char* argv[]) {
int main(void) {
  initscr();
  keypad(stdscr, TRUE); // we need to take in UP,DOWN,LEFT,RIGHT keys and vim-like hjkl keys to scroll
  nonl(); raw(); // modify char input
  curs_set(0);
  noecho();

  int scr_x, scr_y; // screen size variables
  getmaxyx(stdscr, scr_y, scr_x);

  int height = scr_y - (2*v_margin);
  int width = scr_x - (2*h_margin);
  WINDOW* main_window    = newwin(height, width, v_margin, h_margin);
  WINDOW* reading_window = derwin(main_window, height-(v_padding*2), width-(2+h_padding*2), v_padding+1, h_padding+1);

  int quit = 0;
  int ch;
  while(!quit) {
    mvprintw(scr_y-1, 0, "curses_test.c");
    move(scr_y-1, (int)scr_x/2-10);
    box(main_window, 0, 0);
    wprintw(reading_window, "Hello, curses!");
    refresh();
    wrefresh(reading_window);
    wrefresh(main_window);

    ch = getch();
    switch(ch) {
      case 'q': // fallthrough
      case 27:  // ESC key
        quit = 1;
        break;
      case 'c' & 037: // CTRL+C
        printw("Press ");
        attrset(A_BOLD); printw("q");
        attrset(A_NORMAL); printw(" to quit");
        (void)getch();
        break;
    }
    clear();
    wclear(reading_window);
    wclear(main_window);
  }

  // cleanup ncurses
  clear();
  wclear(reading_window);
  wrefresh(reading_window);
  delwin(reading_window);
  endwin();

  return 0;
}
