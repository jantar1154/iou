#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#include "h/display.h"
#include "h/util.h"

#define LINE_W 24

#define OUT_MIN_X 1     // Don't change
#define OUT_MIN_Y 3
#define LAST_Y_OFFSET 2
#define Y_SIZE 5        // how many lines one entry takes up

// Prints a line
void d_print_line() {
    for (int i = 0; i < LINE_W; ++i) {
        printw("-");
    }
    printw("\n");
}

// Prints out every debt from `debt_arr`
// TODO: Handle scrolling here!
// IDEA: use up/down arrow keys or J\K to scroll
void d_print_debts(Debt * debt_arr, const int debt_count) {
    const unsigned int free_y = getmaxy(stdscr) - OUT_MIN_Y - 3;
    // How many entries can you possibly see due to terminal height
    const unsigned short entries_per_page = free_y / Y_SIZE;
    d_clear_cmd_output();
    for (int i = 0; i < min(entries_per_page, debt_count); i++) {
        move(i * Y_SIZE + OUT_MIN_Y, OUT_MIN_X);
        Debt * d = debt_arr + i;
        d_print_line();
        char *str = malloc(sizeof(char) * 0xFF);

        snprintf(str, 0xFF, "[%i]", d->index);
        move(i * Y_SIZE + OUT_MIN_Y + 1, OUT_MIN_X);
        printw(str);

        snprintf(str, 0xFF, "From: %s", d->from);
        move(i * Y_SIZE + OUT_MIN_Y + 2, OUT_MIN_X);
        printw(str);

        snprintf(str, 0xFF, "To: %s", d->to);
        move(i * Y_SIZE + OUT_MIN_Y + 3, OUT_MIN_X);
        printw(str);

        snprintf(str, 0xFF, "Amount: %i %s", d->amount, d->currency);        
        move(i * Y_SIZE + OUT_MIN_Y + 4, OUT_MIN_X);
        printw(str);
        free(str);
    }
    printw("\n");
    d_print_line();
    if (debt_count > entries_per_page) {
        d_message("More pages available! Type page <number> to see them.");
    }
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

// Prints, using ncurses, an additional message not necessarily meaning something went wrong
// Text will appear at MAX_Y - 3
void d_message(const char * message) {
    move(getmaxy(stdscr) - 3, OUT_MIN_X);
    printw(message);
}