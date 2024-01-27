#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "h/input.h"
#include "h/file_handler.h"
#include "h/display.h"

const char * HELP =
    "list:\tlist your debts\n"
    "count:\tprints number of debts you have\n"
    "add:\tadds new entry\n"
    "exit:\texits program\n";

// Handles user input to add new entry into the file
void add(const char * filename) {
    char * str = malloc(0xFF);
    Debt debt;
    printf("From: ");
    fgets(str, sizeof(debt.from), stdin);
    memcpy(debt.from, strtok(str, "\n"), sizeof(debt.from));

    printf("To: ");
    fgets(str, sizeof(debt.to), stdin);
    memcpy(debt.to, strtok(str, "\n"), sizeof(debt.to));

    printf("Amount: ");
    char * amount_s = malloc(0x21);
    fgets(amount_s, 32, stdin);
    const size_t amount_i = atoi(amount_s);
    memcpy(&debt.amount, &amount_i, sizeof(debt.amount));
    free(amount_s);

    printf("Currency: ");
    fgets(str, sizeof(debt.currency), stdin);
    memcpy(debt.currency, strtok(str, "\n"), sizeof(debt.currency));
    fh_add_entry(&debt, filename);

    free(str);
}

void edit(Debt * debt_arr) {
    Debt d;
    size_t index;
    char * str = malloc(0xFF * sizeof(char));
    putchar('\n');
    
    printf("index: ");
    fgets(str, 0xFF * sizeof(char), stdin);
    index = strtol(str, NULL, 10);
    
    printf("From: ");
    fgets(str, 0xFF * sizeof(char), stdin);
    strncpy(d.from, strtok(str, "\n"), sizeof(d.from));

    printf("To: ");
    fgets(str, 0xFF * sizeof(char), stdin);
    strncpy(d.to, strtok(str, "\n"), sizeof(d.to));

    printf("Amount: ");
    fgets(str, 0xFF * sizeof(char), stdin);
    int amount = strtol(str, NULL, 10);
    d.amount = amount;

    printf("Currency: ");
    fgets(str, 0xFF * sizeof(char), stdin);
    strncpy(d.currency, strtok(str, "\n"), sizeof(d.currency));

    fh_edit_entry(index, debt_arr, d);

    free(str);
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
        add(filename);
    } else if (!strcmp(msg, "count")) {
        printf("Number of entries: %u\n", debt_count);
    } else if (!strcmp(msg, "edit")) {
        edit(debt_arr);
    } else if (!strcmp(msg, "exit")) {
        *quit = 1;
    } else {
        printf("Invalid command\n");
    }
}