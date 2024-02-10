#ifndef DEBT_H
#define DEBT_H

typedef struct {
    unsigned int index;
    char to[0x40];
    char from[0x40];
    unsigned int amount;
    char currency[0x10];
} Debt;

#endif // DEBT_H