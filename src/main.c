#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "h/input.h"
#include "h/file_handler.h"
#include "h/debt.h"
#include "h/config.h"
#include "h/display.h"
#include "h/colour.h"

int main(int argc, char ** argv) {
    char * debt_fn = "./debt.dat";
    char * config_fn = "./config.ini";
    if (argc > 1) debt_fn = argv[1];
    if (argc > 2) config_fn = argv[2];
    short quit = 0;

    // Init
    c_init(config_fn);
    initscr();
    clr_init();
    
    // Title text
    d_print_center(0, "IOU - A debt management CLI application");
    d_print_center(1, "Copyright (C) 2024 jantar1154");
    char * command = malloc(0xFF * sizeof(char));

    // Load debt file location
    char * val = malloc(0xFF * sizeof(char));
    c_get_value(config_fn, "debtfilelocation", &val);
    if (strlen(val)) debt_fn = val;

    // Init debt file
    if (!fh_init_file(debt_fn)) fh_create_file(debt_fn);
    Debt * debt_arr = malloc(sizeof(Debt) * fh_get_debt_count(debt_fn));

    // Prompt loop
    while (!quit) {
        debt_arr = realloc(debt_arr, sizeof(Debt) * fh_get_debt_count(debt_fn));

        // Text input located at the bottom
        move(getmaxy(stdscr)-1, OUT_MIN_X);
        clrtoeol();
        attron(COL_YELLOW);
        printw("iou > ");
        attroff(COL_YELLOW);
        
        getnstr(command, 0xFF * sizeof(char));
        if (strlen(command) <= 1) continue;
        command = strtok(command, "\n");
        i_handle_input(debt_arr, command, &quit, debt_fn);
        refresh();
    }

    free(debt_arr);
    free(command);
    free(val);

    refresh();
    endwin();

    return 0;
}