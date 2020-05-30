int param_a()
{
    return 0;
}

int param_R()
{
    return 0;
}

int param_t()
{
    return 0;
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

char* my_strcpy(char* a, char* b) {
    if (my_strlen(a)==my_strlen(b)){
        int i = 0;
  	    while(b[i] != '\0'){
  		a[i]=b[i];
		i++;
	    }
    } else {
        char c[my_strlen(b)];
        int i = 0;
  	    while(b[i] != '\0'){
  		a[i]=b[i];
		i++;
	    }
        a = c;
    }
	return a;
}

char * get_params(int argc, char *argv[])
{
    char temp[4] = "";
    char *params = temp;
    int i = 1;
    int j;
    int k = 0;
    while(i<argc)
    {
        j = 1;
        if (argv[i][0] == '-')
        {
            while (argv[i][j]!='\0')
            {
                temp[k] = argv[i][j];
                k++;
                j++;
            }
        }
        i++;
    }
    temp[k] = '\0';
    return params;
}