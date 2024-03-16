#include "h/colour.h"

#include <ncurses.h>
#include <stdlib.h>

void clr_init() {
     if (!has_colors()) {
        endwin();
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
}