#ifndef CONFIG_H
#define CONFIG_H

void c_init(const char * filename);

void c_get_value(const char * filename, char * key, char ** dest);

#endif // CONFIG_H