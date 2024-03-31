#include <stdlib.h>
#include "minishell.h"

int		delete_envp_arg(char **envp[], char *str)
{
	int		i;
	int		j;
	int		new_len;
	char	**envp_new;

	new_len = count_args(*envp);
	envp_new = ft_calloc(new_len, sizeof(char *));
	if (!envp_new)
		return (MALLOC_ERROR);
	i = 0;
	j = 0;
	while ((*envp)[i])
	{
		if (ft_strnstr((*envp)[i], str, ft_strlen(str)) != NULL)
		{
			free((*envp)[i]);
			i++;
			continue ;
		}
		envp_new[j++] = (*envp)[i++];
	}
	*envp = envp_new;
	return (OK);
}

int ft_unset(char *args[], char **envp[])
{
	int i;
	int err;

	err = OK;
	i = 1;
	while (args[i])
	{
		if (find_envp_arg(*envp, args[i]))
			err = delete_envp_arg(envp, args[i]);
		i++;
	}
	return (err);
}