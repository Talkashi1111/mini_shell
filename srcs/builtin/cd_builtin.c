/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 21:49:54 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/17 19:34:18 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_chdir(t_minishell *info, char *new_path, char *curr_path, char *arg)
{
	int	err;

	if (chdir(new_path) == -1)
	{
		err = errno;
		ft_fprintf(STDERR_FILENO, "cd: %s: %s\n", new_path, strerror(err));
		return (err);
	}
	if (update_or_add_envp(info, "OLDPWD=", curr_path) == MALLOC_ERROR)
		return (MALLOC_ERROR);
	err = ft_getcwd(curr_path, PATH_MAX);
	if (err != OK)
		return (err);
	if (update_or_add_envp(info, "PWD=", curr_path) == MALLOC_ERROR)
		return (MALLOC_ERROR);
	if (arg && ft_strncmp(arg, "-", sizeof("-")) == 0)
		ft_printf("%s\n", curr_path);
	return (OK);
}

int	ft_cd2(t_minishell *info, char *path, char **new_path)
{
	if (ft_strncmp(path, "-", sizeof("-")) == 0)
	{
		*new_path = find_envp_arg(info->envp, "OLDPWD", ft_strlen("OLDPWD"));
		if (*new_path == NULL || *new_path[0] == '\0')
		{
			ft_fprintf(STDERR_FILENO, "cd: OLDPWD not set\n");
			return (NOT_FOUND);
		}
	}
	else
		*new_path = path;
	return (OK);
}

int	ft_cd(char *args[], t_minishell *info)
{
	int		err;
	char	*new_path;
	char	curr_path[PATH_MAX];

	err = ft_getcwd(curr_path, PATH_MAX);
	if (err != OK)
		return (err);
	if (args[1] == NULL || ft_strncmp(args[1], "~", sizeof("~")) == 0)
	{
		new_path = find_envp_arg(info->envp, "HOME", ft_strlen("HOME"));
		if (new_path == NULL)
		{
			ft_fprintf(STDERR_FILENO, "cd: HOME not set\n");
			return (NOT_FOUND);
		}
	}
	else if (args[1] != NULL)
	{
		if (ft_cd2(info, args[1], &new_path) == NOT_FOUND)
			return (NOT_FOUND);
	}
	return (ft_chdir(info, new_path, curr_path, args[1]));
}
