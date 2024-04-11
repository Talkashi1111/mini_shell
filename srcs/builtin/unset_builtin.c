/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:45:35 by achappui          #+#    #+#             */
/*   Updated: 2024/04/11 15:57:44 by achappui         ###   ########.fr       */
/*   Updated: 2024/04/10 13:57:21 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

int		delete_envp_arg(t_minishell *info, char *str)
{
	int		i;
	int		j;
	int		new_len;
	char	**envp_new;

	new_len = count_args(info->envp);
	envp_new = ft_calloc(new_len, sizeof(char *));
	if (!envp_new)
		return (MALLOC_ERROR);
	i = 0;
	j = 0;
	while (info->envp[i])
	{
		if (ft_strnstr(info->envp[i], str, ft_strlen(str)) != NULL)
		{
			free(info->envp[i]);
			i++;
			continue ;
		}
		envp_new[j++] = info->envp[i++];
	}
	info->envp = envp_new;
	return (OK);
}

int ft_unset(char *args[], t_minishell *info)
{
	int i;
	int err;

	err = OK;
	i = 1;
	while (args[i])
	{
		if (find_envp_arg(info->envp, args[i], ft_strlen(args[i])))
			err = delete_envp_arg(info, args[i]);
		i++;
	}
	return (err);
}