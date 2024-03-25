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