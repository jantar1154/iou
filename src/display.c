#include <stdio.h>
#include <ncurses.h>
#include "h/display.h"

#define LINE_W 24

#define OUT_MIN_X 0
#define OUT_MIN_Y 3
#define LAST_Y_OFFSET 2

// Prints a line
void d_print_line() {
    for (int i = 0; i < LINE_W; ++i) {
        putchar('-');
    }
    putchar('\n');
}

// Prints out every debt from `debt_arr`
void d_print_debts(Debt * debt_arr, int debt_count) {
    for (int i = 0; i < debt_count; i++) {
        Debt * d = debt_arr + i;
        d_print_line();
        printf("[%u]\nFrom:\t%s\nTo:\t%s\nAmount:\t%i %s\n",
            d->index, d->from, d->to, d->amount, d->currency
        );
    }
    d_print_line();
}

// Prints last command on (max_y - 2) using ncurses
void d_last_cmd(const char * cmd_str) {
    d_last_del();
    mvprintw(getmaxy(stdscr)-LAST_Y_OFFSET, 0, cmd_str);
}

// Clears the last command printed in `d_last_cmd()`
void d_last_del() {
    move(getmaxy(stdscr)-LAST_Y_OFFSET, 0);
    clrtoeol();
}

// Gets command output
// TODO: Scrolling functionality shall be made here
// IDEA: use up/down arrow keys or J\K to scroll
void d_print_cmd_output(const char * output) {
    d_clear_cmd_output();
    mvprintw(OUT_MIN_Y, OUT_MIN_X, output);
}

// Clear area designated for command output
void d_clear_cmd_output() {
    for (int i = OUT_MIN_Y; i < getmaxy(stdscr); ++i) {
        move(i, 0);
        clrtoeol();
    }
}