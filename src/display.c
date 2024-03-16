#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "h/display.h"
#include "h/colour.h"
#include "h/util.h"

#define LINE_W 24

#define OUT_MIN_Y 3
#define LAST_Y_OFFSET 2 // Where last command should be printed
#define Y_SIZE 5        // How many lines one entry takes up

// Prints a line
void d_print_line() {
    for (int i = 0; i < LINE_W; ++i) {
        printw("-");
    }
    printw("\n");
}

// Prints a message into the center of screen
void d_print_center(int y, const char * message) {
    const unsigned int max_x = getmaxx(stdscr);
    attron(A_BOLD | A_ITALIC | A_STANDOUT | COL_BLUE);
    mvprintw(y, max_x/2 - strlen(message)/2, message);
    attroff(A_BOLD | A_ITALIC | A_STANDOUT | COL_BLUE);
}

// Prints out every debt from `debt_arr`
void d_print_debts(Debt * debt_arr, const int page, const int debt_count) {
    const unsigned int free_y = getmaxy(stdscr) - OUT_MIN_Y - 3;
    // How many entries can you possibly see due to terminal height
    const unsigned short entries_per_page = free_y / Y_SIZE;
    d_clear_cmd_output();
    for (int i = 0; i < min(entries_per_page, debt_count); i++) {
        move(i * Y_SIZE + OUT_MIN_Y, OUT_MIN_X);
        if (debt_count <= i + (page-1) * entries_per_page) break;
        Debt * d = debt_arr + i + (page-1) * entries_per_page;
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
    if (debt_count > entries_per_page) {
        char * str = malloc(0xFF * sizeof(char));
        snprintf(
            str, 0xFF * sizeof(char),
            "More pages available! Type \"page <1 - %i>\" to see them.",
            1 + debt_count / entries_per_page
        );
        d_message(str);
        free(str);
    }
}

// Prints last command on (max_y - 2) using ncurses
void d_last_cmd(const char * cmd_str) {
    d_last_del();
    mvprintw(getmaxy(stdscr)-LAST_Y_OFFSET, OUT_MIN_X, cmd_str);
}

// Clears the last command printed in `d_last_cmd()`
void d_last_del() {
    move(getmaxy(stdscr)-LAST_Y_OFFSET, OUT_MIN_X);
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

// Prints, using ncurses, an additional message
// not necessarily meaning something went wrong
// Text will appear at MAX_Y - 3
void d_message(const char * message) {
    move(getmaxy(stdscr) - 3, OUT_MIN_X);
    attron(COL_BLUE);
    printw(message);
    attroff(COL_BLUE);
}