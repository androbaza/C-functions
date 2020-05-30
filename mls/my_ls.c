#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
//#include "functions_ls.h"

int my_strlen(const char *input)
{
    int i = 0;

    while (input[i] != '\0')
    {
        i++;
    }
    return i;
}

int main(int argc, char *argv[])
{
    // variables
    
    int a_opt = 0;
    int R_opt = 0;
    int t_opt = 0;
    int custom_path = 0;

    // counters
    int i = 1;
    int j;
    int k = 0;
    // int traverser = 0;
    
    // check if number of arguments is correct
    if (argc < 1) return 1;

    // if no input: show original ls
    else if (argc == 1) goto list_current_dir;
   
    // get arguments and path
    while(i<argc)
    {
        // check if this arg is options arg
        if (argv[i][0] == '-')
        {
            while (argv[i][j] != '\0')
            {
                if (argv[i][j] == 'a') a_opt = 1;
                if (argv[i][j] == 'R') R_opt = 1;
                if (argv[i][j] == 't') t_opt = 1;
                j++;
            }
        }  
        // else it is a path
        else
        {
            char temp[my_strlen(argv[i])];
            strcpy(temp, argv[i]);
            char *directory_path = temp;
            custom_path = 1;
        } 
        i++;
        j=1;
    }

    list_current_dir:
    if (custom_path == 0)
    {
        char *directory_path = ".";
        printf("%s\n", directory_path); 
        //list_dir(directory_path);   
    }

    

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