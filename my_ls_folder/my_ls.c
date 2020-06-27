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
// #include "functions_ls.h"

typedef struct opts
{
    int a;
    int R;
    int t;
} opts;


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

int is_regular(const char *path)
{
    struct stat buf;
    if (stat(path, &buf) != 0)
    {
        printf("ls: cannot access '%s': No such file or directory\n", path);
        return 2;
    }
    return S_ISREG(buf.st_mode);
}

// void display_contents(const char* file)
// {
//     printf("%s  ", file);
//     return;
// }

void list_contents(const char *path, opts *options)
{
    DIR* directory = opendir(path);

    if(directory == NULL)
    {
        // printf("ls: cannot access '%s': No such file or directory\n", path);
        return;
    }

    struct dirent * curr_file_struct = readdir(directory);

    // take 256 as initial guess for # of items in dir
    size_t contents_num = 256;
    size_t index = 0;
    size_t display_index = 0;

    // array of char pointers to save the names of contents
    char **contents_array = malloc(sizeof(char*) * contents_num);

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
                contents_array = realloc(contents_array, sizeof(char*) * contents_num);
            }
        contents_array[index] = curr_file_struct->d_name;
        curr_file_struct = readdir(directory);
        index++;
    }

    // here it will be sorted by t
    
    while (display_index<index)
    {
        printf("%s  ", contents_array[display_index]);
        // display_contents(contents_array[display_index]);
        display_index++;
    }

    closedir(directory);
    free(contents_array);
}

int main(int argc, char *argv[])
{
    // variables
    opts *options = (opts *)malloc(sizeof(opts));
    options->a = 0;
    options->t = 0;

    // array of string pointer with maximum possible number of arguments of argc
    char **directory_operands = malloc(sizeof(char*) * argc);
    char **reg_file_operands = malloc(sizeof(char*) * argc);

    size_t dir_index = 0;
    size_t reg_file_index = 0;
    size_t error_index = 0;

    // counters for arguments
    size_t index_word = 1;
    size_t index_symb = 1;
    
    // check if number of arguments is correct
    if (argc < 1) return 1;
   
    // get arguments and path
    while(index_word < (size_t)argc)
    {
        // check if this arg is options arg
        if (argv[index_word][0] == '-')
        {
            while (argv[index_word][index_symb] != '\0')
            {
                if (argv[index_word][index_symb] == 'a') 
                    options->a = 1;
                else if (argv[index_word][index_symb] == 't') 
                    options->t = 1;
                else 
                    printf("ls: invalid option -- '%c'\n", argv[index_word][index_symb]);
                index_symb++;
            }
        }
        // else it is a path to directory or a file
        else
        {   
            if(is_regular(argv[index_word]) == 2)
            {
                // if the file does not exist
                index_word++;
                error_index++;
                index_symb = 1;
                continue;
            }
            else if (is_regular(argv[index_word]) == 1)
            {
                // longest directory path is 255 chars + \0
                reg_file_operands[reg_file_index] = malloc(256 * sizeof(char));
                my_strcpy(reg_file_operands[reg_file_index], argv[index_word]);
                reg_file_index++;
            }
            else
            {
                directory_operands[dir_index] = malloc(256 * sizeof(char));
                my_strcpy(directory_operands[dir_index], argv[index_word]);
                dir_index++;   
            }
        }
        index_word++;
        index_symb = 1;
    }

    // if no path operands, write default '.'
    if(dir_index + reg_file_index + error_index == 0)
    {
        list_contents(".", options);
    }
    else
    // first show reg files, then directories
    {
        index_word = 0;
        while(index_word < reg_file_index)
        {
            printf("%s  ", reg_file_operands[index_word]);
            if((index_word+1 == reg_file_index) && dir_index != 0)
                printf("\n\n");
            index_word++;
        }

        index_word = 0;
        while(index_word < dir_index)
        {
            if (dir_index > 1)
                printf("%s:\n", directory_operands[index_word]);
            list_contents(directory_operands[index_word], options);
            if(index_word+1 != dir_index)
                printf("\n\n");
            index_word++;
        }
    }

    if (error_index == 0)
        printf("\n");

    // free mallocked variables
    for(size_t i = 0; i < dir_index; i++)
        free(directory_operands[i]);

    for(size_t i = 0; i < reg_file_index; i++)
        free(reg_file_operands[i]);

    free(options);
    free(directory_operands);
    free(reg_file_operands);
}

// to-do: lexi sort, time sort

// -Wall -Wextra -Werror
// gimli -verbose my_ls.c

// if no input: show original ls
// else if (argc == 1) goto list_current_dir;