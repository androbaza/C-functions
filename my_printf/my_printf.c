#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

static const char BASE_SYMBOLS[] = "0123456789abcdef";
static const char MINUS[] = "-";

char *change_base(unsigned int input, int base)  // and convert to string
{
	char buffer[21];  // UINT_MAX is 10 bytes. Take +1 just in case
	char *counter;
	
	counter = &buffer[20]; 
	*counter = '\0';  //s tart building string from the end.
	
    if (input == 0)  // in case the input is 0, just insert it without any division
    {
        counter--;
		*counter = '0';
        return counter;
    }

	while(input != 0)  // divide the input by the base untill 0 is left
    {
		counter--;
		* counter = BASE_SYMBOLS[input % base]; 
		input /= base; 
    }
	return counter; 
}

char * pointer_to_hex(unsigned long input, int base)  // and convert to string
{ 
	char buffer[15];  // UINT_MAX is 10 bytes. Take +1 just in case
	char * counter;  

	counter = &buffer[14]; 
	*counter = '\0';  // start building string from the end.
	while(input != 0)  // divide the input by the base untill 0 is left
    {
		counter--;
		*counter = BASE_SYMBOLS[input%base]; 
		input /= base; 
    }
    counter = buffer;
    buffer[0] = '0';
    buffer[1] = 'x';
	return counter; 
}


int my_strlen(const char *input)
{
    int i = 0;

    while (input[i] != '\0')
    {
        i++;
    }
    return i;
}


int my_printf(char *restrict format, ...){

	int num_of_chars = 0, x = 0, bytes_to_write = 0;  // var for counting # of chars inputted, var for argument
    const char *output;  // pointer to string for arguments to go to output 
	const char *input_char = format;  // pointer to the first element of input

	va_list print_arg;  // init the variable for arguments of printf
	va_start(print_arg, format);

	while (*input_char != '\0')
    {
        if ((input_char == format) & (*input_char != '%'))  // safety check to avoid memory leak for the 0th character
        {
            num_of_chars += write(1, input_char, 1);
		    input_char++;
            continue;
        }

        if ((input_char == format) & (*input_char == '%'))  // safety check to avoid memory leak in case the input starts from %
        {
		    input_char++;
            continue;
        }

        if ((*input_char != '%') & (*(input_char - 1) != '%'))  // write to stdout in case of plain text
        {
            num_of_chars += write(1, input_char, 1);
		    input_char++;
            continue;
        }

        else if (*input_char == '%')  // go to next character after %
        {
            input_char++; 
            continue;
        }

        else if ((*(input_char - 1) == '%') & (strchr("douxcsp", *(input_char)) == NULL))  // if the conversion specification is not defined print ERROR
        {
            my_printf("\nERROR\nunknown conversion type character '%c'\n", *(input_char));
            return 1;
        }

        else if (*(input_char - 1) == '%')  // fetch the conversion specification
        {
            if (*input_char == 'd') 
            {  // decimal case
                x = va_arg(print_arg, int);
                if (x < 0)
                {
                    num_of_chars += write(1, MINUS, 1);
                    x = -x;
                }
                output = change_base(x, 10);
                bytes_to_write = strlen(output);
		    }      

            if (*input_char == 'o')
            { // octal
                x = va_arg(print_arg, int);
                unsigned int x_unsigned = (unsigned int) x;
                output = change_base(x_unsigned, 8);
                bytes_to_write = strlen(output);
            }

            if (*input_char == 'u')
            {  // unsigned decimal
                x = va_arg(print_arg, int);
                unsigned int x_unsigned = (unsigned int) x;
                output = change_base(x_unsigned, 10);
                bytes_to_write = strlen(output);
            }

            if (*input_char == 'x')
            { // hex
                x = va_arg(print_arg, int);
                unsigned int x_unsigned = (unsigned int) x;
                output = change_base(x_unsigned, 16);
                bytes_to_write = strlen(output);
            }

            if (*input_char == 'c')
            { //c har
                char temp[2];
                temp[0] = (char) va_arg(print_arg, int);
                temp[1] = '\0';
                output = temp;
                bytes_to_write = 1;
            }

            if (*input_char == 's')
            { // string
                const char *x_s = va_arg(print_arg, char *);
                if (x_s == (char *)NULL)
                {
                    const char * x_null = "(null)";
                    output = x_null;
                }
                else output = x_s;
                bytes_to_write = my_strlen(output);
            }

            if (*input_char == 'p')
            { // pointer
                unsigned long z = (unsigned long)va_arg(print_arg, void *);
                output = pointer_to_hex(z, 16);
                bytes_to_write = strlen(output);
            }

            num_of_chars += write(1, output, bytes_to_write);  // write the converted string
            input_char++;
        }
	} 
	va_end(print_arg); 
	return num_of_chars;
}