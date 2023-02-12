#include <dlfcn.h>
#include <syscfg.h>
#include "config.h"


char **syscfg_list(const char *key) {
    // open the module .so
    void *so_handle = dlopen(SYSCFG_MODULE_PATH, RTLD_LAZY);
    if (so_handle == NULL)
        return NULL;

    // get a pointer to the syscfg_list function
    char **(*_syscfg_list)(const char *) = dlsym(so_handle, "m_syscfg_list");

    // run the function
    char **result = (*_syscfg_list)(key);

    // close the .so and return
    dlclose(so_handle);
    return result;
}

size_t syscfg_get(const char *key, const char *opt, char **val_ptr) {
    // open the module .so
    void *so_handle = dlopen(SYSCFG_MODULE_PATH, RTLD_LAZY);
    if (so_handle == NULL)
        return 0;

    // get a pointer to the syscfg_get function
    size_t (*_syscfg_get)(const char *, const char *, char *const *) = dlsym(so_handle, "m_syscfg_get");

    // run the function
    size_t result = (*_syscfg_get)(key, opt, val_ptr);

    // close the .so and return
    dlclose(so_handle);
    return result;
}

size_t syscfg_set(const char *key, const char *opt, char *str) {
    // open the module .so
    void *so_handle = dlopen(SYSCFG_MODULE_PATH, RTLD_LAZY);
    if (so_handle == NULL)
        return -1;

    // get a pointer to the syscfg_set function
    size_t (*_syscfg_set)(const char *, const char *, char *) = dlsym(so_handle, "m_syscfg_set");

    // run the function
    size_t result = (*_syscfg_set)(key, opt, str);

    // close the .so and return
    dlclose(so_handle);
    return result;
}