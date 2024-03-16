#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include "h/input.h"
#include "h/file_handler.h"
#include "h/display.h"
#include "h/colour.h"
#include "h/util.h"

#define HELP \
    "HELP: " \
    "help:\tshows this help\n" \
    "list:\tlist your debts\n" \
    "query:\tlists debts matching a search\n" \
    "page:\tchanges page of list you are currently viewing\n" \
    "count:\tprints number of debts you have\n" \
    "add:\tadds new entry\n" \
    "edit:\tedits existing entry\n" \
    "clear:\tclears the screen\n" \
    "exit:\texits program\n" \

static int open_text = 0;
unsigned int res_size = 0;
static Debt * result;

// Handles user input to add new entry into the file
void i_add(Debt * debt_arr, const char * filename) {
    const size_t str_size = 0xFF;
    char * str = malloc(sizeof(char) * str_size);
    strcpy(str, "");

    Debt debt; // Debt to add
    debt.index = fh_get_debt_count(filename);
    attron(COL_YELLOW);
    d_print_cmd_output("Create new entry:\n");
    attroff(COL_YELLOW);
    printw("From: ");
    while (!strlen(str)) getnstr(str, str_size);
    strcpy(debt.from, strtok(str, "\n"));
    strcpy(str, "");

    printw("To: ");
    while (!strlen(str)) getnstr(str, str_size);
    strcpy(debt.to, strtok(str, "\n"));
    strcpy(str, "");

    printw("Amount: ");
    while (0 == strlen(str)) getnstr(str, str_size);
    const size_t amount_i = atoi(str);
    memcpy(&debt.amount, &amount_i, sizeof(debt.amount));
    strcpy(str, "");

    printw("Currency: ");
    while (!strlen(str)) getnstr(str, str_size);
    memcpy(debt.currency, strtok(str, "\n"), sizeof(debt.currency));
    strcpy(str, "");
    fh_add_entry(&debt, filename);
    d_print_cmd_output("New entry successfully added!");

    free(str);
}

void i_remove_entry(Debt * debt_arr, const char * filename) {
    char * input = malloc(sizeof(char) * 0xFF);
    const int arr_size = fh_get_debt_count(filename);
    if (arr_size <= 0) {
        attron(COL_RED);
        d_print_cmd_output("No entries found. Nothing to remove");
        attroff(COL_RED);
        free(input);
        return;
    }
    d_print_cmd_output("Index: ");
    getnstr(input, 0xFF);
    const int index = strtol(input, NULL, 10);
    free(input);
    if (index > fh_get_last_id(filename)) {
        char * str = malloc(sizeof(char) * 0xFF);
        attron(COL_RED);
        snprintf(str, 0xFF, "Index %i out of range!", index);
        attroff(COL_RED);
        d_print_cmd_output(str);
        free(str);
        return;
    }

    fh_remove_entry(filename, debt_arr, index);
    char * str = malloc(sizeof(char) * 0xFF);
    attron(COL_GREEN);
    snprintf(str, 0xFF, "Entry %i removed successfully!", index);
    attroff(COL_GREEN);
    d_print_cmd_output(str);
    free(str);
}

// Gets input from user to edit an entry
void i_edit(Debt * debt_arr, const char * filename) {
    Debt d;
    char * str = malloc(0xFF * sizeof(char));
    
    d_print_cmd_output("index: ");
    getnstr(str, 0xFF);
    const unsigned int index = strtol(str, NULL, 10);
    if (!fh_index_exists(debt_arr, filename, index)) {
        attron(COL_RED);
        d_print_cmd_output("Index out of range\n");
        attroff(COL_RED);
        free(str);
        return;
    }
    const Debt * og = fh_debt_by_id(debt_arr, filename, index);
    
    printw("From: %s -> ", og->from);
    getnstr(str, 0xFF);
    if (strlen(str) == 0) strcpy(d.from, og->from);
    else strcpy(d.from, strtok(str, "\n"));

    printw("To: %s -> ", og->to);
    getnstr(str, 0xFF);
    if (strlen(str) == 0) strcpy(d.to, og->to);
    else strcpy(d.to, strtok(str, "\n"));

    printw("Amount: %i -> ", og->amount);
    getnstr(str, 0xFF);
    int amount = strtol(str, NULL, 10);
    if (strlen(str) == 0) amount = og->amount;
    d.amount = amount;

    printw("Currency: %s -> ", og->currency);
    getnstr(str, 0xFF);
    if (strlen(str) == 0) strcpy(d.currency, og->currency);
    else strcpy(d.currency, strtok(str, "\n"));

    fh_edit_entry(index, debt_arr, d, filename);

    free(str);
    attron(COL_GREEN);
    d_message("Entry edited successfully!");
    attroff(COL_GREEN);
}

void i_query(const char * filename, Debt * debt_arr) {
    const int count = fh_get_debt_count(filename);
    char * input = malloc(sizeof(char) * 0xFF);
    const char * txt =
        "Search for:\n"
        "1: From\n"
        "2: To\n"
        "3: Amount\n"
        "4: Currency\n";

    char str[0xFF];
    snprintf(str, 0xFF, "%s\nSelect: ", txt);
    d_print_cmd_output(str);
    getnstr(input, 0xFF);
    const int pick = strtol(input, NULL, 10);
    if (pick < 1 || pick > 4) {
        attron(COL_RED);
        d_print_cmd_output("Selection invalid. Use numbers 1 - 4\n");
        attroff(COL_RED);
        free(input);
        return;
    }

    printw("Search: ");
    getnstr(input, 0xFF);
    const char * search = strtok(input, "\n");

    res_size = 0;
    result = fh_query(debt_arr, count, pick, search, &res_size);
    d_print_debts(result, 1, res_size);
    free(input);
}

// Handles input commands
void i_handle_input(
Debt * debt_arr, const char * msg, short * quit, const char * filename) {
    const unsigned short debt_count = fh_get_debt_count(filename);
    unsigned int page = 1;
    bool invalid = false;
    char * msg_mut = calloc(sizeof(char), 0xFF);
    strncpy(msg_mut, msg, 0xFF);
    if (!strcmp(strtok(msg_mut, " "), "list")) {
        if (!debt_count) {
            attron(COL_RED);
            d_print_cmd_output("No entries found!");
            attroff(COL_RED);
        } else {
            fh_read_file(debt_arr, filename);
            d_print_debts(debt_arr, max(1, page), debt_count);
            open_text = 0;
        }
    } else if (!strcmp(msg_mut, "page")) {
        char * n = strtok(NULL, "\n");
        if (!n) n = "1";
        page = strtol(n, NULL, 10);
        switch (open_text) {
            case 0: // list
                d_print_debts(debt_arr, max(1, page), debt_count);
                break;
            case 1: // query
                d_print_debts(result, page, res_size);
                break;
        }
    } else if (!strcmp(msg_mut, "help")) {
        char * str = malloc(sizeof(char) * 0xFFF);
        sprintf(str, "\n%s\n", HELP);
        d_print_cmd_output(str);
        free(str);
    } else if (!strcmp(msg_mut, "add")) {
        i_add(debt_arr, filename);
    } else if (!strcmp(msg_mut, "count")) {
        char *str = malloc(sizeof(char) * 0x40);
        snprintf(str, sizeof(char) * 0x40, "Number of entries: %i", debt_count);
        d_print_cmd_output(str);
        free(str);
    } else if (!strcmp(msg_mut, "edit")) {
        fh_read_file(debt_arr, filename);
        i_edit(debt_arr, filename);
    } else if (!strcmp(msg_mut, "query")) {
        fh_read_file(debt_arr, filename);
        i_query(filename, debt_arr);
        open_text = 1;
    } else if (!strcmp(msg_mut, "remove")) {
        i_remove_entry(debt_arr, filename);
    } else if (!strcmp(msg, "exit")) {
        free(result);
        *quit = 1;
    } else if (!strcmp(msg_mut, "clear")) {
        d_clear_cmd_output();
    } else {
        invalid = true;
    }
    free(msg_mut);
    if (invalid) {
        attron(COL_RED);
        d_last_cmd("Invalid command");
        attroff(COL_RED);
    } else {
        d_last_cmd(msg);
    }
}