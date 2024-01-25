#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "h/input.h"
#include "h/file_handler.h"

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
    const unsigned int amount_i = atoi(amount_s);
    printf("amout_i: %i\n", amount_i);
    memcpy(&debt.amount, &amount_i, sizeof(debt.amount));
    free(amount_s);

    printf("Currency: ");
    fgets(str, sizeof(debt.currency), stdin);
    memcpy(debt.currency, strtok(str, "\n"), sizeof(debt.currency));
    add_entry(&debt, filename);

    free(str);
}

// Handles input commands
void handle_input(Debt * debt_arr, const char * msg, short * quit, const char * filename) {
    if (!strcmp(msg, "list")) {
        if (get_debt_count(filename)) {
            read_file(debt_arr, filename);
        } else {
            puts("No entries found");
        }
    } else if (!strcmp(msg, "help")) {
        printf("%s", HELP);
    } else if (!strcmp(msg, "add")) {
        add(filename);
    } else if (!strcmp(msg, "count")) {
        printf("Number of entries: %u\n", get_debt_count(filename));
    } else if (!strcmp(msg, "exit")) {
        *quit = 1;
    } else {
        printf("Invalid command\n");
    }
}