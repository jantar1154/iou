#ifndef DISPLAY_H
#define DISPLAY_H

#include "debt.h"

#define C_RED     "\x1b[31m"
#define C_YELLOW  "\x1b[33m"
#define C_GREEN   "\x1b[32m"
#define C_BLUE    "\x1b[34m"
#define C_RESET   "\x1b[0m"

void d_print_debts(Debt * debt_arr, int debt_count);

void d_last_cmd(const char * cmd_str);

void d_last_del();

void d_print_cmd_output(const char * output);

void d_clear_cmd_output();

#endif // DISPLAY_H