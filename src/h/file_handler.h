#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdbool.h>
#include "debt.h"

bool fh_init_file(const char * filename);

void fh_create_file(const char * filename);

unsigned short fh_get_debt_count(const char * filename);

void fh_read_file(Debt * debt_arr, const char * filename);

void fh_add_entry(Debt * entry, const char * filename);

void fh_edit_entry(unsigned int index, Debt * debt_arr, const Debt replace, const char * filename);

void fh_remove_entry(const char * filename, Debt * debt_arr, const int index);

Debt * fh_query(Debt * debt_arr, int count, int query_type, const char * search, unsigned int * res_size);

bool fh_index_exists(Debt * debt_arr, const char * filename, unsigned int index);

Debt * fh_debt_by_id(Debt * debt_arr, const char * filename, unsigned int index);

int fh_get_last_id(const char * filename);

#endif // FILE_HANDLER_H