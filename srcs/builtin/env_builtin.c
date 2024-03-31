#include <unistd.h>
#include "minishell.h"

char	**copy_env(char *envp[])
{
	int i;
	char **envp_cpy;

	i = count_args(envp);
	envp_cpy = ft_calloc(i + 1, sizeof(char *));
	if (!envp_cpy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_cpy[i] = ft_strdup(envp[i]);
		if (!envp_cpy[i])
		{
			free_args(envp_cpy);
			return (NULL);
		}
		i++;
	}
	return (envp_cpy);
}

int	ft_env(char *args[], char **envp[])
{
	int i;

	if (args[1] != NULL)
	{
		ft_fprintf(STDERR_FILENO, "env: too many arguments\n");
		return (USAGE_ERROR);
	}
	i = 0;
	while ((*envp)[i])
	{
		ft_printf("%s\n", (*envp)[i]);
		i++;
	}
	return (OK);
}