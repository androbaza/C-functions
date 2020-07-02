#include <stddef.h>

typedef struct opts
{
    int a;
    int R;
    int t;
} opts;

#ifndef FUNCTIONS_LS
#define FUNCTIONS_LS

void my_strcpy(char *output, const char *input);

int my_strlen(const char *input);

int my_strcmp(const char *s1, const char *s2);

int my_timecmp(const char *s1, const char *s2);

int is_regular(const char *path);

void display_contents(char **contents_array, const size_t index);

void strcpy_for_sort(char *s1, char *s2);

char** sort_arr_str(char **contents_array, const size_t size, const opts *options);

void list_contents(const char *path, const opts *options);

#endif