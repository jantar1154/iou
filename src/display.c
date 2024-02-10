#include <stdio.h>
#include "h/display.h"

#define LINE_W 24

// Prints a line
void d_print_line() {
    for (int i = 0; i < LINE_W; ++i) {
        putchar('-');
    }
    putchar('\n');
}

// Prints out every debt from `debt_arr`
void d_print_debts(const Debt * debt_arr, int debt_count) {
    for (int i = 0; i < debt_count; i++) {
        Debt * d = debt_arr + i;
        d_print_line();
        printf("[%i]\nFrom:\t%s\nTo:\t%s\nAmount:\t%i %s\n",
            i+1, d->from, d->to, d->amount, d->currency
        );
    }
    d_print_line();
}