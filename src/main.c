#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h/input.h"
#include "h/file_handler.h"
#include "h/debt.h"

#define FILE_NAME "debt.dat"

/*
    FILE HIEARCHY
    two bytes: number of debts (entries) (ushort)
    148 bytes: binary format to read into "h/debt.h/Debt" struct
    every other entry is 148 bytes (sizeof(Debt))
*/

int main() {
    short quit = 0;

    char * command = malloc(0xFF * sizeof(char));
    if (!fh_init_file(FILE_NAME)) fh_create_file(FILE_NAME);
    Debt * debt_arr = malloc(sizeof(Debt) * fh_get_debt_count(FILE_NAME));

    while (!quit) {
        debt_arr = realloc(debt_arr, sizeof(Debt) * fh_get_debt_count(FILE_NAME));
        fprintf(stdout, "iou > ");
        fgets(command, 0xFF * sizeof(char), stdin);
        if (strlen(command) <= 1) continue;
        command = strtok(command, "\n");
        i_handle_input(debt_arr, command, &quit, FILE_NAME);
    }
    free(debt_arr);
    free(command);

    return 0;
}