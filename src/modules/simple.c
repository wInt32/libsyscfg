#include <stdio.h>
#include <stddef.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include "../config.h"

char *_resolve_key_path(const char *key){
    char *key_path = SYSCFG_M_SIMPLE_CFG_PATH;

    if (key != NULL && key != "") {
        char *old_key_path = key_path;
        size_t old_key_path_len = strlen(old_key_path);
        size_t key_len = strlen(key);
        key_path = calloc(old_key_path_len+key_len+1, sizeof(char));
        if (key_path == NULL)
            return NULL;
        strcat(key_path, old_key_path);
        strcat(key_path, key);
    }
}

char *_resolve_opt_path(const char *key, const char *opt){
    char *key_path = _resolve_key_path(key);

    if (key != NULL && key != "") {
        char *old_key_path = key_path;
        size_t old_key_path_len = strlen(old_key_path);
        size_t opt_len = strlen(opt);
        key_path = calloc(old_key_path_len+opt_len+1, sizeof(char));
        if (key_path == NULL)
            return NULL;
        strcat(key_path, old_key_path);
        strcat(key_path, opt);
        free(old_key_path);
        
    }
    return key_path;
}


char **syscfg_list(const char *key) {
    char *key_path = _resolve_key_path(key);
    DIR *key_dir = opendir(key_path);
    if (key_dir == NULL)
        return NULL;
    size_t entry_count = 0;
    struct dirent *dent = readdir(key_dir);
    while (dent != NULL) {
        entry_count++;
        dent = readdir(key_dir);
    }
    entry_count -= 2;

    rewinddir(key_dir);

    char **result = calloc(entry_count+1, sizeof (char *));
    for (int i = 0; i < entry_count; i++) {
        dent = readdir(key_dir);
        if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) {
            i--;
            continue;
        }
            
        
        int name_sz = strlen(dent->d_name)+1;
        result[i] = calloc(name_sz, sizeof(char));
        memcpy(result[i], dent->d_name, name_sz);
        
    }
    result[entry_count] == NULL;

    closedir(key_dir);
    free(key_path);
    return result;
}

size_t syscfg_get(const char *key, const char *opt, char **val_ptr) {
    char *opt_path = _resolve_opt_path(key, opt);
    FILE *opt_file = fopen(opt_path, "r");
    if (opt_file == NULL)
        return 0;

    fseek(opt_file, 0, SEEK_END)+1;
    size_t length = ftell(opt_file);
    rewind(opt_file);
    *val_ptr = calloc(length+1, sizeof(char));
    if (val_ptr == NULL)
        return 0;

    return fread(*val_ptr, sizeof(char), length, opt_file);
}

size_t syscfg_set(const char *key, const char *opt, char *str) {
    char *opt_path = _resolve_opt_path(key, opt);
    FILE *opt_file = fopen(opt_path, "w");
    if (opt_file == NULL)
        return 0;

    return fwrite(str, sizeof(char), strlen(str), opt_file);
}