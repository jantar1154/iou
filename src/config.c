#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "h/config.h"

FILE * c_create_config_file(const char * filename) {
    FILE * config_file = fopen(filename, "w");
    if (!config_file) {
        fprintf(stderr, "Could not create file %s!\n", filename);
        exit(1);
    }
    return config_file;
}

// Gets value of `key` from `filename` and copies it into `dest`
// If it doesn't find `key`, copies nothing
void c_get_value(const char * filename, char * key, char ** dest) {
    FILE * config_file = fopen(filename, "r");
    char * buffer = malloc(0xFF * sizeof(char));
    while (fgets(buffer, (0xFF * sizeof(char)), config_file)) {
        char * tk = strtok(buffer, ":");
        if (strcmp(tk, key)) continue;
        char * value = strtok(NULL, "\n");
        strcpy(*dest, value);
    }
    free(buffer);
    fclose(config_file);
}

// Initialises config file
void c_init(const char * filename) {
    FILE * config_file = fopen(filename, "r");
    if (!config_file) config_file = c_create_config_file(filename);
    fprintf(config_file, "debtfile:debt.dat\n");
    fprintf(config_file, "kokot:yes\n");
    fclose(config_file);
}