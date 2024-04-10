/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:45:07 by achappui          #+#    #+#             */
/*   Updated: 2024/04/10 13:57:07 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "minishell.h"


int is_valid_export(char *str)
{
	char *end_key;

	end_key = ft_strchr(str, '=');
	if (!end_key || end_key == str)
		return (USAGE_ERROR);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (USAGE_ERROR);
	str++;
	while (str < end_key)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (USAGE_ERROR);
		str++;
	}
	return (OK);
}

int	print_sorted_envp(char *envp[])
{
	char	**sorted_envp;
	int		i;
	int		j;
	char	*tmp;

	sorted_envp = copy_env(envp);
	if (!sorted_envp)
		return (MALLOC_ERROR);
	i = 0;
	while (sorted_envp[i])
	{
		j = i + 1;
		while (sorted_envp[j])
		{
			if (ft_strncmp(sorted_envp[i], sorted_envp[j], ft_strlen(sorted_envp[i])) > 0)
			{
				tmp = sorted_envp[i];
				sorted_envp[i] = sorted_envp[j];
				sorted_envp[j] = tmp;
			}
			j++;
		}
		ft_printf("declare -x %s\n", sorted_envp[i]);
		i++;
	}
	free_args(sorted_envp);
	return (OK);
}

int ft_export(char *args[], t_minishell *info)
{
	int	i;
	char	*key;
	char	*value;
	int	err;

	if (!args[1])
	{
		print_sorted_envp(info->envp);
		return (OK);
	}
	err = OK;
	i = 1;
	while (args[i])
	{
		if (is_valid_export(args[i]) == USAGE_ERROR)
		{
			ft_fprintf(STDERR_FILENO, "export: `%s': not a valid identifier\n", args[i]);
			err = USAGE_ERROR;
			i++;
			continue ;
		}
		value = ft_strchr(args[i], '=') + 1;
		key = ft_substr(args[i], 0, value - args[i]);
		if (!key || update_or_add_envp(info, key, value) == MALLOC_ERROR)
			err = MALLOC_ERROR;
		free(key);
		i++;
	}
	return (err);
}