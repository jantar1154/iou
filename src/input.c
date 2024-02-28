#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include "h/input.h"
#include "h/file_handler.h"
#include "h/display.h"

const char * HELP =
    "help:\tshows this help\n"
    "list:\tlist your debts\n"
    "query:\tlists debts matching a search\n"
    "count:\tprints number of debts you have\n"
    "add:\tadds new entry\n"
    "edit:\tedits existing entry\n"
    "clear:\tclears the screen\n"
    "exit:\texits program\n";

// Handles user input to add new entry into the file
void i_add(Debt * debt_arr, const char * filename) {
    const size_t str_size = 0xFF;
    char * str = malloc(sizeof(char) * str_size);

    Debt debt; // Debt to add
    debt.index = fh_get_debt_count(filename);
    d_print_cmd_output("Create new entry:\n");
    printw("From: ");
    getnstr(str, str_size);
    strcpy(debt.from, strtok(str, "\n"));

    printw("To: ");
    getnstr(str, str_size);
    strcpy(debt.to, strtok(str, "\n"));

    printw("Amount: ");
    char * amount_s = malloc(sizeof(char) * 0x20);
    getnstr(amount_s, 0x20);
    const size_t amount_i = atoi(amount_s);
    memcpy(&debt.amount, &amount_i, sizeof(debt.amount));
    free(amount_s);

    printw("Currency: ");
    getnstr(str, str_size);
    memcpy(debt.currency, strtok(str, "\n"), sizeof(debt.currency));
    fh_add_entry(&debt, filename);
    d_print_cmd_output("New entry successfully added!");

    free(str);
}

void i_remove_entry(Debt * debt_arr, const char * filename) {
    char * input = malloc(sizeof(char) * 0xFF);
    const int arr_size = fh_get_debt_count(filename);
    if (arr_size <= 0) {
        puts(C_YELLOW "No entries found. Nothing to remove" C_RESET);
        free(input);
        return;
    }
    printf("Index: ");
    fgets(input, sizeof(char) * 0xFF, stdin);
    const int index = strtol(input, NULL, 10);
    free(input);
    if (index > fh_get_last_id(filename)) {
        printf(C_RED "Index %i out of range!" C_RESET "\n", index);
        return;
    }

    fh_remove_entry(filename, debt_arr, index);
    printf(C_GREEN "Entry %i removed successfully!" C_RESET "\n", index);
}

// Gets input from user to edit an entry
void i_edit(Debt * debt_arr, const char * filename) {
    Debt d;
    char * str = malloc(0xFF * sizeof(char));
    putchar('\n');
    
    printf("index: ");
    fgets(str, 0xFF * sizeof(char), stdin);
    const unsigned int index = strtol(str, NULL, 10);
    if (!fh_index_exists(debt_arr, filename, index)) {
        puts("Index out of range");
        free(str);
        return;
    }
    const Debt * og = fh_debt_by_id(debt_arr, filename, index);
    
    printf("From: %s -> ", og->from);
    fgets(str, 0xFF * sizeof(char), stdin);
    if (strlen(str) == 1) strcpy(d.from, og->from);
    else strcpy(d.from, strtok(str, "\n"));

    printf("To: %s -> ", og->to);
    fgets(str, 0xFF * sizeof(char), stdin);
    if (strlen(str) == 1) strcpy(d.to, og->to);
    else strcpy(d.to, strtok(str, "\n"));

    printf("Amount: %i -> ", og->amount);
    fgets(str, 0xFF * sizeof(char), stdin);
    int amount = strtol(str, NULL, 10);
    if (strlen(str) == 1) amount = og->amount;
    d.amount = amount;

    printf("Currency: %s -> ", og->currency);
    fgets(str, 0xFF * sizeof(char), stdin);
    if (strlen(str) == 1) strcpy(d.currency, og->currency);
    else strcpy(d.currency, strtok(str, "\n"));

    fh_edit_entry(index, debt_arr, d, filename);

    free(str);
    puts(C_GREEN "Entry edited successfully!" C_RESET);
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

    printf("%s\nSelect: ", txt);
    fgets(input, sizeof(char) * 0xFF, stdin);
    const int pick = strtol(input, NULL, 10);
    if (pick < 1 || pick > 4) {
        fprintf(stderr, C_RED
            "Selection %i is invalid. Use numbers 1 - 4" C_RESET "\n", pick);
        free(input);
        return;
    }

    printf("Search: ");
    fgets(input, sizeof(char) * 0xFF, stdin);
    const char * search = strtok(input, "\n");

    unsigned int res_size = 0;
    Debt * result = fh_query(debt_arr, count, pick, search, &res_size);
    d_print_debts(result, res_size);
    free(result);
    free(input);
}

// Handles input commands
void i_handle_input(
Debt * debt_arr, const char * msg, short * quit, const char * filename) {
    const unsigned short debt_count = fh_get_debt_count(filename);
    bool invalid = false;
    if (!strcmp(msg, "list")) {
        if (!debt_count) {
            d_print_cmd_output("No entries found!");
        } else {
            fh_read_file(debt_arr, filename);
            d_print_debts(debt_arr, debt_count);
        }
    } else if (!strcmp(msg, "help")) {
        char str[0xFF];
        sprintf(str, "\n%s\n", HELP);
        d_print_cmd_output(str);
    } else if (!strcmp(msg, "add")) {
        i_add(debt_arr, filename);
    } else if (!strcmp(msg, "count")) {
        char *str = malloc(sizeof(char) * 0x40);
        snprintf(str, sizeof(char) * 0x40, "Number of entries: %i", debt_count);
        d_print_cmd_output(str);
        free(str);
    } else if (!strcmp(msg, "edit")) {
        i_edit(debt_arr, filename);
    } else if (!strcmp(msg, "query")) {
        i_query(filename, debt_arr);
    } else if (!strcmp(msg, "remove")) {
        i_remove_entry(debt_arr, filename);
    } else if (!strcmp(msg, "exit")) {
        *quit = 1;
    } else if (!strcmp(msg, "clear")) {
        d_clear_cmd_output();
    } else {
        invalid = true;
    }
    d_last_cmd(invalid ? "Invalid command" : msg);
}