 #include "../minishell.h"



ssize_t ftgetline(char **lineptr, size_t *n)
{
    size_t i;
    char buffer[2000];
    ssize_t bytes_read;

    i = 0;
    if (!*lineptr)
    {
        *lineptr = malloc(90000);
        if (!*lineptr)
            return -1;
        *n = 128;
    }
    while (1)
    {
        bytes_read = read(STDIN_FILENO, buffer, 1999);
        if (bytes_read <= 0) //keep reding unuk we hit newline or ned of nput 
            return -1;
        buffer[bytes_read] = 0;
        i = 0;
        while (i < (size_t)bytes_read)
        {
            (*lineptr)[i] = buffer[i];
            if (buffer[i++] == '\n')
            {
                (*lineptr)[i] = 0;
                handle_input(*lineptr, lineptr);  
                return i;
            }
        }
    }
}

void handle_input(char *input, char **lineptr)
{
    if (!ft_strncmp(input, "exit", 4))
    {
        free(*lineptr);
        printf(  "Exiting  ...\n");
        exit(0);
    }
    ft_printf("entered : ->  ");
    write(1, input, ft_strlen(input));
    write(1, "\n", 1);
}
