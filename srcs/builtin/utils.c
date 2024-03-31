#include <stdlib.h>
#include "minishell.h"

int count_args(char **args)
{
    int i;

    if (!args)
        return (0);
    i = 0;
    while (args[i])
        i++;
    return (i);
}

void    free_args(char **args)
{
    int i;

    if (!args)
        return ;
    i = 0;
    while (args[i])
    {
        free(args[i]);
        args[i] = NULL;
        i++;
    }
    free(args);
}