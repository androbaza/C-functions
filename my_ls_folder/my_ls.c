#include <stdlib.h>
#include <stdio.h>
#include "functions_ls.h"

int main(int argc, char *argv[])
{
    // struct for options
    opts *options = (opts *)malloc(sizeof(opts));
    options->a = 0;
    options->t = 0;

    // array of string pointer with maximum possible number of arguments of argc
    char **directory_operands = malloc(sizeof(char *) * argc);
    char **reg_file_operands = malloc(sizeof(char *) * argc);

    // counters for operands and false queries
    size_t dir_index = 0;
    size_t reg_file_index = 0;
    size_t error_index = 0;

    // counters for arguments
    size_t index_word = 1;
    size_t index_symb = 1;
    
    // check if number of arguments is correct
    if (argc < 1) return 1;
   
    // get arguments and path
    while (index_word < (size_t)argc)
    {
        // check if this arg is options arg
        if (argv[index_word][0] == '-')
        {
            while (argv[index_word][index_symb] != '\0')
            {
                if (argv[index_word][index_symb] == 'a')
                {
                    options->a = 1;
                } 
                else if (argv[index_word][index_symb] == 't') 
                {
                    options->t = 1;
                }
                else 
                {
                    printf("ls: invalid option -- '%c'\n", argv[index_word][index_symb]);
                }
                index_symb++;
            }
        }
        // else it is a path to directory or a file
        else
        {   
            if (is_regular(argv[index_word]) == 2)
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
    if (dir_index + reg_file_index + error_index == 0)
    {
        list_contents(".", options);
    }
    else
    // first show reg files, then directories
    {
        index_word = 0;
        sort_arr_str(reg_file_operands, reg_file_index, options);
        while (index_word < reg_file_index)
        {
            printf("%s  ", reg_file_operands[index_word]);
            if ((index_word+1 == reg_file_index) && dir_index != 0)
            {
                printf("\n\n");
            }
            index_word++;
        }

        index_word = 0;
        sort_arr_str(directory_operands, dir_index, options);
        while (index_word < dir_index)
        {
            if (dir_index > 1)
            {
                printf("%s:\n", directory_operands[index_word]);
            }
            list_contents(directory_operands[index_word], options);
            if (index_word + 1 != dir_index)
            {
                printf("\n\n");
            }
            index_word++;
        }
    }

    if (error_index == 0)
    {
        printf("\n");
    }

    // free mallocked variables
    for (size_t i = 0; i < dir_index; i++)
    {
        free(directory_operands[i]);
    }

    for (size_t i = 0; i < reg_file_index; i++)
    {
        free(reg_file_operands[i]);
    }

    free(options);
    free(directory_operands);
    free(reg_file_operands);
}