#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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
    "exit:\texits program\n";

// Handles user input to add new entry into the file
void i_add(Debt * debt_arr, const char * filename) {
    char * str = malloc(0xFF);
    Debt debt; // Debt to add
    debt.index = fh_get_debt_count(filename);
    printf("From: ");
    fgets(str, sizeof(debt.from), stdin);
    strcpy(debt.from, strtok(str, "\n"));

    printf("To: ");
    fgets(str, sizeof(debt.to), stdin);
    strcpy(debt.to, strtok(str, "\n"));

    printf("Amount: ");
    char * amount_s = malloc(sizeof(char) * 0x20);
    fgets(amount_s, sizeof(char) * 0x20, stdin);
    const size_t amount_i = atoi(amount_s);
    memcpy(&debt.amount, &amount_i, sizeof(debt.amount));
    free(amount_s);

    printf("Currency: ");
    fgets(str, sizeof(debt.currency), stdin);
    memcpy(debt.currency, strtok(str, "\n"), sizeof(debt.currency));
    fh_add_entry(&debt, filename);
    printf("New entry successfully added!\n");

    free(str);
}

void i_remove_entry(Debt * debt_arr, const char * filename) {
    char * input = malloc(sizeof(char) * 0xFF);
    const int arr_size = fh_get_debt_count(filename);
    if (arr_size <= 0) {
        fputs("No entries found. Nothing to remove\n", stderr);
        free(input);
        return;
    }
    printf("Index: ");
    fgets(input, sizeof(char) * 0xFF, stdin);
    const int index = strtol(input, NULL, 10);
    free(input);
    if (index > arr_size) {
        printf("Index %i out of range!\n", index);
        return;
    }

    fh_remove_entry(filename, debt_arr, index);
    printf("Entry %i removed successfully!\n", index);
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
        fprintf(stderr, "Selection %i is invalid. Use numbers 1 - 4\n", pick);
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
void i_handle_input(Debt * debt_arr, const char * msg, short * quit, const char * filename) {
    unsigned short debt_count = fh_get_debt_count(filename);
    if (!strcmp(msg, "list")) {
        if (debt_count) {
            fh_read_file(debt_arr, filename);
            d_print_debts(debt_arr, debt_count);
        } else {
            puts("No entries found");
        }
    } else if (!strcmp(msg, "help")) {
        printf("%s", HELP);
    } else if (!strcmp(msg, "add")) {
        i_add(debt_arr, filename);
    } else if (!strcmp(msg, "count")) {
        printf("Number of entries: %u\n", debt_count);
    } else if (!strcmp(msg, "edit")) {
        i_edit(debt_arr, filename);
    } else if (!strcmp(msg, "query")) {
        i_query(filename, debt_arr);
    } else if (!strcmp(msg, "remove")) {
        i_remove_entry(debt_arr, filename);
    } else if (!strcmp(msg, "exit")) {
        *quit = 1;
    } else {
        puts("Invalid command");
    }
}