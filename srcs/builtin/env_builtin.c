/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:44:57 by achappui          #+#    #+#             */
/*   Updated: 2024/05/07 14:45:09 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char *envp[])
{
	int		i;
	char	**envp_cpy;

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

int	ft_env(char *args[], t_minishell *info)
{
	int	i;

	if (args[1] != NULL)
	{
		ft_fprintf(STDERR_FILENO, "env: too many arguments\n");
		return (USAGE_ERROR);
	}
	i = 0;
	while (info->envp[i])
	{
		ft_printf("%s\n", info->envp[i]);
		i++;
	}
	return (OK);
}
