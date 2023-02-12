#ifndef __SYSCFG_H
#include <stddef.h>

/* returns the available keys/opts for key */
char **syscfg_list(const char *key);

/* stores the pointer to the value of opt in key to *val_ptr */
size_t syscfg_get(const char *key, const char *opt, char **val_ptr);

/* sets opt in key to val_buf */
size_t syscfg_set(const char *key, const char *opt, char *str);

#endif