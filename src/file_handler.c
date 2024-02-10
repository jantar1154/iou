#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "h/file_handler.h"
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
// @return A number value of first two bytes in file
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

// Appends new entry into a savefile
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

// Removes an entry from file by index
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
    fseek(debt_file, offset, SEEK_SET);
    fwrite(arr_after, sizeof(Debt), count_after, debt_file);
    fclose(debt_file);
}

// Compares `search` with it's corresponding value from `debt` based on `query_type`
// @return `true` if `debt` has the same value as `search`, otherwise `false`
bool fh_q_compare(const Debt * debt, const char * search, int query_type) {
    switch(query_type) {
        case 1:
            return 0 == strcmp(search, debt->from);
        case 2:
            return 0 == strcmp(search, debt->to);
        case 3:
            const int search_int = strtol(search, NULL, 10);
            return search_int == debt->amount;
        case 4:
            return 0 == strcmp(search, debt->currency);
        default:
            return false;
    }
}

// Searches for entries from `debt_arr` matching `search`
// @return An array of matching entries from `debt_arr`
Debt * fh_query(Debt * debt_arr, int count, int query_type, const char * search, unsigned int * res_size) {
    Debt * result_arr = malloc(sizeof(Debt) * count);
    unsigned int result_size = 0;
    for (int i = 0; i < count; ++i) {
        const Debt * d = debt_arr + i;
        bool should_add = fh_q_compare(d, search, query_type);
        if (!should_add) continue;
        memcpy(result_arr + result_size, d, sizeof(Debt));
        result_size ++;
    }
    memcpy(res_size, &result_size, sizeof(unsigned int));
    return result_arr;
}