#ifndef DEBT_H
#define DEBT_H

typedef struct {
    char to[0x40];
    char from[0x40];
    unsigned int amount;
    char currency[0x10];
} Debt;

#endif // DEBT_H