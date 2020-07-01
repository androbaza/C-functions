#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "functions_ls.h"

void my_strcpy(char *output, const char *input)
{
    size_t index = 0;

    while (input[index] != '\0')
    {
        output[index] = input[index];
        index++;
    }
    output[index] = '\0';
}
 
int my_strlen(const char *input)
{
    size_t index = 0;

    while (input[index] != '\0')
    {
        index++;
    }
    return index;
}

int my_strcmp(const char *s1, const char *s2) 
{
    while (*s1 != '\0')
    {
        // if the chars dont match
        if (*s1 != *s2)
        {
            return *s1 - *s2;
        }

        s1++;
        s2++;

        // if one of the strings is shorter
        if (((*s1 == '\0') | (*s2 == '\0')) && *s1 != *s2)
        {
                return *s1 - *s2;
        }
    }
    // if equal
    return 0;
}

int my_timecmp(const char *s1, const char *s2)
{
    struct stat buf1;
    struct stat buf2;

    stat (s1, &buf1);
    stat (s2, &buf2);

     if (buf1.st_mtim.tv_sec == buf2.st_mtim.tv_sec)
    {
        return buf2.st_mtim.tv_nsec > buf1.st_mtim.tv_nsec;
    }
    return buf2.st_mtim.tv_sec > buf1.st_mtim.tv_sec;
}

int is_regular(const char *path)
{
    struct stat buf;
    if(stat(path, &buf) != 0)
    {
        printf("ls: cannot access '%s': No such file or directory\n", path);
        return 2;
    }
    return S_ISREG(buf.st_mode);
}

void display_contents(char **contents_array, const size_t index)
{
    size_t display_index = 0;

    while (display_index<index)
    {
        if (index > 10)
        {
            printf("%s\n", contents_array[display_index]);
            display_index++;
            continue;
        }
        printf("%s  ", contents_array[display_index]);
        display_index++;
    }
}

void strcpy_for_sort(char *s1, char *s2)
{
    char temp[256];

    my_strcpy(temp, s1);
    my_strcpy(s1, s2);
    my_strcpy(s2, temp);
}

char** sort_arr_str(char **contents_array, const size_t size, const opts *options)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = i+1; j < size; j++)
        {
            // sort by time
            if (options->t == 1)
            {
                if (my_timecmp(contents_array[i], contents_array[j]) > 0)
                {
                    strcpy_for_sort(contents_array[i], contents_array[j]);
                }
                // if time is the same, sort by lex
                else if ((my_timecmp(contents_array[i], contents_array[j]) == 0) && (my_strcmp(contents_array[i], contents_array[j]) > 0))
                {
                    strcpy_for_sort(contents_array[i], contents_array[j]);
                }
            }
            // else sort by lex
            else if (my_strcmp(contents_array[i], contents_array[j]) > 0)
            {
                    strcpy_for_sort(contents_array[i], contents_array[j]);
            }
        }
    }
    return contents_array;
} 

void list_contents(const char *path, const opts *options)
{
    DIR* directory = opendir(path);

    if (directory == NULL)
    {
        return;
    }

    struct dirent * curr_file_struct = readdir(directory);

    // take 256 as initial guess for # of items in dir
    size_t contents_num = 256;
    size_t index = 0;

    // array of char pointers to save the names of contents
    char **contents_array = malloc(sizeof(char *) * contents_num);

    // write contents to our array of strings  
    while (curr_file_struct != NULL)
    {
        // do not show hidden files if no option -a
        if ((options->a == 0) && (curr_file_struct->d_name[0] == '.'))
        {
            curr_file_struct = readdir(directory);
            continue;
        }
        // if there are more items than we allocated in memory, double the size
        if (index >= contents_num)
            {
                contents_num *= 2;
                contents_array = realloc(contents_array, sizeof(char *) * contents_num);
            }
        contents_array[index] = curr_file_struct->d_name;
        curr_file_struct = readdir(directory);
        index++;
    }

    display_contents(sort_arr_str(contents_array, index, options), index);
    closedir(directory);
    free(contents_array);
}