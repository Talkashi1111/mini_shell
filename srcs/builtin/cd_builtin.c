/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:44:37 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 20:22:40 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

int ft_cd(char *args[], t_minishell *info)
{
	int		err;
	char	*new_path;
	char	curr_path[PATH_MAX];

	err = ft_getcwd(curr_path, sizeof(curr_path));
	if (err != OK)
		return (err);
	if (args[1] == NULL || ft_strncmp(args[1], "~", sizeof("~")) == 0)
	{
		new_path = find_envp_arg(info->envp, "HOME", 0);
		if (new_path == NULL)
		{
			ft_fprintf(STDERR_FILENO, "cd: HOME not set\n");
			return (NOT_FOUND);
		}
	}
	else if (args[1] != NULL)
	{
		if (ft_strncmp(args[1], "-", sizeof("-")) == 0)
		{
			new_path = find_envp_arg(info->envp, "OLDPWD", 0);
			if (new_path == NULL || new_path[0] == '\0')
			{
				ft_fprintf(STDERR_FILENO, "cd: OLDPWD not set\n");
				return (NOT_FOUND);
			}
		}
		else
			new_path = args[1];
	}
	if (chdir(new_path) == -1)
	{
		err = errno;
		ft_fprintf(STDERR_FILENO, "cd: %s: %s\n", new_path, strerror(err));
		return (err);
	}
	if (update_or_add_envp(info, "OLDPWD=", curr_path) == MALLOC_ERROR)
		return (MALLOC_ERROR);
	err = ft_getcwd(curr_path, sizeof(curr_path));
	if (err != OK)
		return (err);
	if (update_or_add_envp(info, "PWD=", curr_path) == MALLOC_ERROR)
		return (MALLOC_ERROR);
	return (OK);
}


