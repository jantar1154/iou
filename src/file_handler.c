#include "h/file_handler.h"
#include <stdio.h>

#include "h/debt.h"

// Checks if debt_file exists
// @return `1` if file exists or `0` if it does not
int fh_init_file(const char * filename) {
    FILE * debt_file = fopen(filename, "rb");
    int rtn = 1;
    if (!debt_file) rtn = 0;
    if (debt_file) fclose(debt_file);
    return rtn;
}

// Creates ready empty file
void fh_create_file(const char * filename) {
    FILE * debt_file = fopen(filename, "wb"); // "touch"
    // Initialise value for debt count
    unsigned short count = 0;
    fwrite(&count, sizeof(unsigned short), 1, debt_file);
    fclose(debt_file);
}

// Reads integer giving you number of debts in file
unsigned short fh_get_debt_count(const char * filename) {
    FILE * debt_file = fopen(filename, "rb");
    unsigned short count;
    fread(&count, 2, 1, debt_file);
    fclose(debt_file);
    return count;
}

// Reads the file and populates debt_arr
void fh_read_file(Debt * debt_arr, const char * filename) {
    const unsigned short arr_size = fh_get_debt_count(filename);
    FILE * debt_file = fopen(filename, "rb");
    // Offset it to not read debt count
    fseek(debt_file, sizeof(unsigned short), SEEK_SET);
    // Populate `debt_arr`
    for (size_t i = 0; i < arr_size && fread(debt_arr+i, sizeof(Debt), 1, debt_file); ++i);
    fclose(debt_file);
}

void fh_add_entry(Debt * entry, const char * filename) {
    unsigned short count = fh_get_debt_count(filename) + 1;
    const int offset = (count - 1) * sizeof(Debt) + sizeof(unsigned short);
    
    FILE * debt_file = fopen(filename, "r+b");
    // Increase debt count number
    fwrite(&count, sizeof(unsigned short), 1, debt_file);

    // Append new entry
    fseek(debt_file, offset, SEEK_SET);
    fwrite(entry, sizeof(Debt), 1, debt_file);
    fclose(debt_file);
}

// Edits already existing entry in file
void fh_edit_entry(const int index, Debt * debt_arr, const Debt replace, const char * filename) {
    Debt * debt_to_replace = debt_arr + index;
    *debt_to_replace = replace;
    FILE * debt_file = fopen(filename, "r+b");
    const int offset = sizeof(unsigned short) + sizeof(Debt) * index;
    fseek(debt_file, offset, SEEK_SET);
    fwrite(debt_to_replace, sizeof(Debt), 1, debt_file);
    fclose(debt_file);
}

void fh_remove_entry(const char * filename, Debt * debt_arr, const int index) {
    const unsigned short arr_size = fh_get_debt_count(filename);
    const int count_after = arr_size - index - 1;
    Debt * arr_after = debt_arr + index + 1;
    const int offset = index + sizeof(unsigned short);
    const unsigned short new_count = arr_size - 1;

    FILE * debt_file = fopen(filename, "r+b");
    // Change number of entries
    fseek(debt_file, 0, SEEK_SET);
    fwrite(&new_count, sizeof(unsigned short), 1, debt_file);
    // Write everything else
    printf("after: %i\n", count_after);
    fseek(debt_file, offset, SEEK_SET);
    fwrite(arr_after, sizeof(Debt), count_after, debt_file);
    fclose(debt_file);
}