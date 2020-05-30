#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "functions_ls.h"


int main(int argc, char *argv[])
{
    // variables
    char *start_directory_path = (char *)calloc(10, sizeof(char));
    if (start_directory_path == NULL)
    {
        return 1;
    }
    int traverser = 0;

    // default starting directory
    start_directory_path = ".\0";

    // check if number of arguments is correct
    if (argc < 1)
    {
        return 1;
    }
   
    // get arguments
    //get_arguments(argc, argv);
    

    free(start_directory_path);
    return 0;
}