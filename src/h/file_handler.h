#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "debt.h"

int fh_init_file(const char * filename);

void fh_create_file(const char * filename);

unsigned short fh_get_debt_count(const char * filename);

void fh_read_file(Debt * debt_arr, const char * filename);

void fh_add_entry(Debt * entry, const char * filename);

void fh_edit_entry(const int index, Debt * debt_arr, const Debt replace);

#endif // FILE_HANDLER_H