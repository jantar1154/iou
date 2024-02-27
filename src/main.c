#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "h/input.h"
#include "h/file_handler.h"
#include "h/debt.h"
#include "h/config.h"
#include "h/display.h"

int main(int argc, char ** argv) {
    char * debt_fn = "./debt.dat";
    char * config_fn = "./config.ini";
    if (argc > 1) debt_fn = argv[1];
    if (argc > 2) config_fn = argv[2];
    short quit = 0;

    // Init
    c_init(config_fn);
    initscr();
    
    // Title text
    const char * title_text = "IOU - Copyright (C) 2024 jantar1154\n";
    unsigned int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    attron(A_BOLD | A_ITALIC);
    mvprintw(0, max_x/2 - strlen(title_text)/2, title_text);
    refresh();
    attroff(A_BOLD | A_ITALIC);

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
        move(max_y-1, 0);
        clrtoeol();
        printw("iou > ");
        
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