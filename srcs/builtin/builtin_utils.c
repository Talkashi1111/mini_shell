#include <stdlib.h>
#include "minishell.h"

char *find_envp_arg(char *envp[], char *str)
{
	int i;
	int len;

	len = ft_strlen(str);
	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], str, len) != NULL)
			return (envp[i] + len);
		i++;
	}
	return (NULL);
}

int update_or_add_envp(char **envp[], char *str, char *new_val)
{
	int i;
	char **envp_new;
	char *new_entry;

	new_entry = ft_strjoin(str, new_val);
	if (!new_entry)
		return (MALLOC_ERROR);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strnstr((*envp)[i], str, ft_strlen(str)) != NULL)
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return (OK);
		}
		i++;
	}
	envp_new = ft_calloc(i + 2, sizeof(char *));
	if (!envp_new)
	{
		free(new_entry);
		return (MALLOC_ERROR);
	}
	i = 0;
	while ((*envp)[i])
	{
		envp_new[i] = (*envp)[i];
		i++;
	}
	envp_new[i] = new_entry;
	free(*envp);
	*envp = envp_new;
	return (OK);
}