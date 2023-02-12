#include <stdio.h>
#include <stddef.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include "../config.h"

// returns the real path to a key
char *_resolve_key_path(const char *key){

    // use the configured config root
    char *key_path = SYSCFG_M_SIMPLE_CFG_PATH;

    // if the key is NULL just use the default
    if (key != NULL && key != "") {
        char *old_key_path = key_path;

        size_t old_key_path_len = strlen(old_key_path);
        size_t key_len = strlen(key);

        // we need to free() this later!
        key_path = calloc(old_key_path_len+key_len+1, sizeof(char));
        if (key_path == NULL)
            return NULL;

        // no need to use strncat, this should be safe
        strcat(key_path, old_key_path);
        strcat(key_path, key);
    }
}


// returns the real path to an option within a key
char *_resolve_opt_path(const char *key, const char *opt){

    // get the real key path
    char *key_path = _resolve_key_path(key);

    // if the key is NULL just use the default
    if (key != NULL && key != "") {
        char *old_key_path = key_path;

        size_t old_key_path_len = strlen(old_key_path);
        size_t opt_len = strlen(opt);

        // we need to free() this later!
        key_path = calloc(old_key_path_len+opt_len+1, sizeof(char));

        if (key_path == NULL)
            return NULL;

        // should still be safe
        strcat(key_path, old_key_path);
        strcat(key_path, opt);

        // freeing the old key
        free(old_key_path);
        
    }
    return key_path;
}

// lists the keys/opts for key
char **syscfg_list(const char *key) {

    // get the real key path and open the key
    char *key_path = _resolve_key_path(key);
    DIR *key_dir = opendir(key_path);

    if (key_dir == NULL)
        return NULL;
    
    // # of keys/opts
    size_t entry_count = 0;

    // a single entry
    struct dirent *dent = readdir(key_dir);

    // count the entries
    while (dent != NULL) {
        entry_count++;
        dent = readdir(key_dir);
    }
    // don't count . and ..
    entry_count -= 2;

    // go to the beginning of the directory
    rewinddir(key_dir);

    // allocate enough space for entry_count pointers + NULL
    char **result = calloc(entry_count+1, sizeof (char *));

    for (int i = 0; i < entry_count; i++) {
        dent = readdir(key_dir);

        // skip . and ..
        if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) {
            i--;
            continue;
        }
            
        int name_sz = strlen(dent->d_name)+1;

        // allocate space for the entry, we need to free() this!
        result[i] = calloc(name_sz, sizeof(char));

        // copy the entry to the allocated space
        memcpy(result[i], dent->d_name, name_sz);
        
    }

    // last entry should be NULL, just like in **argv
    result[entry_count] == NULL;

    // close the directory and free the key string
    closedir(key_dir);
    free(key_path);

    return result;
}

// get value of opt in key to char *val pointed to by val_ptr
size_t syscfg_get(const char *key, const char *opt, char **val_ptr) {

    // get the real opt path and open the file for reading
    char *opt_path = _resolve_opt_path(key, opt);
    FILE *opt_file = fopen(opt_path, "r");

    if (opt_file == NULL)
        return 0;

    // get the size of the file
    fseek(opt_file, 0, SEEK_END)+1;
    size_t length = ftell(opt_file);
    rewind(opt_file);

    // set *val_ptr to point to a newly allocated space, we need to free() this!
    *val_ptr = calloc(length+1, sizeof(char));

    if (val_ptr == NULL)
        return 0;

    // read opt and return the # of bytes read, close the file
    size_t res = fread(*val_ptr, sizeof(char), length, opt_file);
    fclose(opt_file);
    return res;
}

// write str to opt in key
size_t syscfg_set(const char *key, const char *opt, char *str) {
    // get the real opt path and open the file for writing
    char *opt_path = _resolve_opt_path(key, opt);
    FILE *opt_file = fopen(opt_path, "w");

    if (opt_file == NULL)
        return 0;

    // write str to opt and return the # of bytes written, close the file
    size_t res = fwrite(str, sizeof(char), strlen(str), opt_file);
    fclose(opt_file);
    return res;
}