#include <stdlib.h>
#include "minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v'
		|| c == '\r' || c == '\f' || c == '\n')
		return (TRUE);
	return (FALSE);
}

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