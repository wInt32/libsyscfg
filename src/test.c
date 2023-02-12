#include <stdio.h>
#include <syscfg.h>
#include <malloc.h>
#include "config.h"

void print_str_array(char **str_arr){
	printf("[");
	for (int i = 0; str_arr[i] != NULL; i++){
		printf("\"%s\", ", str_arr[i]);
	}
	printf("NULL]\n");
}


int main(int argc, char **argv) {

    printf("Testing list...\n");
    char **list_result = syscfg_list("");
    print_str_array(list_result);

    printf("Testing get...\n");
    char *val;
    size_t get_result = syscfg_get("system/", "test", &val);
    printf("%s\n", val);
    printf("Read %u bytes.\n", get_result);

    printf("Testing set...\n");
    size_t set_result = syscfg_set("system/", "afile", "test");
    printf("Set %u bytes.\n", set_result);

    printf("Done.\n");
}