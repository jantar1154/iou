#include "h/file_handler.h"
#include <stdio.h>

#include "h/debt.h"

// Checks if debt_file exists
int init_file(const char * filename) {
    FILE * debt_file = fopen(filename, "rb");
    int rtn = 1;
    if (!debt_file) rtn = 0;
    if (debt_file) fclose(debt_file);
    return rtn;
}

// Creates ready empty file
void create_file(const char * filename) {
    FILE * debt_file = fopen(filename, "wb"); // touch
    // Initialise value for debt count
    unsigned short count = 0;
    fwrite(&count, sizeof(unsigned short), 1, debt_file);
    fclose(debt_file);
}

// Reads integer giving you number of debts in file
unsigned short get_debt_count(const char * filename) {
    FILE * debt_file = fopen(filename, "rb");
    unsigned short count;
    fread(&count, 2, 1, debt_file);
    fclose(debt_file);
    return count;
}

// Reads the file and populates debt_arr
void read_file(Debt * debt_arr, const char * filename) {
    FILE * debt_file = fopen(filename, "rb");
    // Offset it to not read debt count
    fseek(debt_file, sizeof(unsigned short), SEEK_SET);
    for (int i = 0; fread(debt_arr+i, sizeof(Debt), 1, debt_file); ++i) {
        Debt * d = debt_arr + i;
        printf("From: %s\nTo: %s\nAmount: %u %s\n", d->from, d->to, d->amount, d->currency);
    }
    fclose(debt_file);
}

void add_entry(Debt * entry, const char * filename) {
    unsigned short count = get_debt_count(filename) + 1;
    FILE * debt_file = fopen(filename, "r+b");
    // Increase debt count number
    fwrite(&count, sizeof(unsigned short), 1, debt_file);

    // Append new entry
    fseek(debt_file, 0, SEEK_END);
    fwrite(entry, sizeof(Debt), 1, debt_file);
    fclose(debt_file);
}