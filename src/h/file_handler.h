#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "debt.h"

int init_file(const char * filename);

void create_file(const char * filename);

unsigned short get_debt_count(const char * filename);

void read_file(Debt * debt_arr, const char * filename);

void add_entry(Debt * entry, const char * filename);

void edit_entry(const int index, Debt * debt_arr, const Debt replace);

#endif // FILE_HANDLER_H