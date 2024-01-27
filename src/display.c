#include <stdio.h>
#include "h/display.h"

// Prints out every debt from `debt_arr`
void d_print_debts(Debt * debt_arr, int debt_count) {
    putchar('\n');
    for (int i = 0; i < debt_count; i++) {
        Debt * d = debt_arr + i;
        printf("[%i]\nFrom:\t%s\nTo:\t%s\nAmount:\t%i %s\n\n",
            i+1, d->from, d->to, d->amount, d->currency
        );
    }
}