#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
//#include "functions_ls.h"


typedef struct opts
{
    int a;
    int R;
    int t;
} opts;


void my_strcpy(const char *input, char *output)
{
    int index = 0;

    while (input[index] != '\0')
    {
        output[index] = input[index];
        index++;
    }
    output[index] = '\0';
}
 
int my_strlen(const char *input)
{
    int index = 0;

    while (input[index] != '\0')
    {
        index++;
    }
    return index;
}


int main(int argc, char *argv[])
{
    // variables
    opts *options = (opts *)malloc(sizeof(opts));
    options->a = 0;
    options->R = 0;
    options->t = 0;

    //longest directory path is 255 chars + \0
    char *directory_path = malloc(256 * sizeof(char));
    my_strcpy(directory_path, ".");
    int custom_path = 0;

    // counters for arguments
    int index_word = 1;
    int index_symb = 1;
    
    // check if number of arguments is correct
    if (argc < 1) return 1;

    // if no input: show original ls
    //else if (argc == 1) goto list_current_dir;
   
    // get arguments and path
    while(index_word<argc)
    {
        // check if this arg is options arg
        if (argv[index_word][0] == '-')
        {
            while (argv[index_word][index_symb] != '\0')
            {
                if (argv[index_word][index_symb] == 'a') options->a = 1;
                else if (argv[index_word][index_symb] == 'R') options->R = 1;
                else if (argv[index_word][index_symb] == 't') options->t = 1;
                index_symb++;
            }
        }  
        // else it is a path to directory or a file
        else
        {
            my_strcpy(directory_path, argv[index_word]);
            custom_path = 1;
        } 
        index_word++;
        index_symb=1;
    }

    printf("path: %s\na: %i\nR: %i\nt: %i\n", directory_path, options->a, options->R, options->t); 

    //list_current_dir:
    // if (custom_path == 0)
    // {
    //     char *directory_path = ".";
    //     printf("%s\n", directory_path); 
    //     //list_dir(directory_path);   
    // }

    //free mallocked variables
    free(options);
    free(directory_path);
    return 0;
}

// -Wall -Wextra -Werror
// gimli -verbose my_ls.c


 // max path length is 4096 + 1 for \0
    //char *directory_path = (char *)malloc(40*sizeof(char)); 
    /*if (directory_path == NULL)
    {
        return 1;
    }
    */
    // default starting directory
    // strcpy(directory_path, ".");