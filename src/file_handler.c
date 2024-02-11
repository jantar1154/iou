#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "h/file_handler.h"
#include "h/debt.h"

// Checks if debt_file exists
// @return `true` if file exists or `false` if it does not
bool fh_init_file(const char * filename) {
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

// Reads the file and populates `debt_arr`
void fh_read_file(Debt * debt_arr, const char * filename) {
    const unsigned short arr_size = fh_get_debt_count(filename);
    FILE * debt_file = fopen(filename, "rb");
    // Offset it to not read debt count
    fseek(debt_file, sizeof(unsigned short), SEEK_SET);
    // Populate `debt_arr`
    for (
        size_t i = 0;
        i < arr_size && fread(debt_arr+i, sizeof(Debt), 1, debt_file);
        ++i
    );
    fclose(debt_file);
}

// Appends new entry into a savefile
// TODO: fix this, absolutely does not work!! important
void fh_add_entry(Debt * entry, const char * filename) {
    const unsigned int arr_size = fh_get_debt_count(filename);
    const unsigned short new_size = arr_size + 1;
    const unsigned short new_id = fh_get_last_id(filename) + 1;
    const int offset = arr_size * sizeof(Debt) + sizeof(unsigned short);
    
    entry->index = new_id;
    FILE * debt_file = fopen(filename, "r+b");
    // Increase debt count number
    fwrite(&new_size, sizeof(unsigned short), 1, debt_file);

    // Append new entry
    fseek(debt_file, offset, SEEK_SET);
    fwrite(entry, sizeof(Debt), 1, debt_file);
    fclose(debt_file);
}

// Edits already existing entry in file
void fh_edit_entry(
unsigned int index,
Debt * debt_arr,
const Debt replace,
const char * filename) {
    Debt * debt_to_replace = fh_debt_by_id(debt_arr, filename, index);
    memcpy(debt_to_replace, &replace, sizeof(Debt));
    debt_to_replace->index = index;
    const unsigned int count = fh_get_debt_count(filename);
    FILE * debt_file = fopen(filename, "r+b");
    const int offset = sizeof(unsigned short);
    fseek(debt_file, offset, SEEK_SET);
    fwrite(debt_arr, sizeof(Debt), count, debt_file);
    fclose(debt_file);
}

// Removes an entry from file by index
void fh_remove_entry(const char * filename, Debt * debt_arr, const int index) {
    const unsigned short arr_size = fh_get_debt_count(filename);
    const int count_after = arr_size - index - 1;
    Debt * arr_after = debt_arr + index + 1;
    const int offset = sizeof(Debt) * index + sizeof(unsigned short);
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

// Compares `search` with it's corresponding value from `debt`
// based on `query_type`
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

bool fh_index_exists(
Debt * debt_arr,
const char * filename,
unsigned int index) {
    const unsigned int count = fh_get_debt_count(filename);
    if (index < 0) return false;
    for (int i = 0; i < count; ++i) {
        Debt * d = debt_arr + i;
        if (d->index == index) {
            return true;
        }
    }
    return false;
}

// Finds a debt in `debt_arr` whose index is `index`
// @return An array to first found debt with the right index
// @return or NULL if none was found
Debt * fh_debt_by_id(
Debt * debt_arr,
const char * filename,
unsigned int index) {
    const unsigned int count = fh_get_debt_count(filename);
    for (int i = 0; i < count; ++i) {
        Debt * d = debt_arr + i;
        if (d->index != index) continue;
        return d;
    }
    return NULL;
}

// Finds last (largest) index in `debt_arr`
int fh_get_last_id(const char * filename) {
    const unsigned int count = fh_get_debt_count(filename);
    Debt * debt_arr = malloc(sizeof(Debt) * count);
    fh_read_file(debt_arr, filename);
    int largest = -1;
    for (int i = 0; i < count; ++i) {
        const int curr_id = (debt_arr + i)->index;
        if (curr_id > largest) largest = curr_id;
    }
    free(debt_arr);
    return largest;
}

// Searches for entries from `debt_arr` matching `search`
// @return An array of matching entries from `debt_arr`
Debt * fh_query(
Debt * debt_arr,
int count,
int query_type,
const char * search,
unsigned int * res_size) {
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