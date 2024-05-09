/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 21:49:54 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/07 14:44:54 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_chdir(t_minishell *info, char *new_path, char *curr_path, char *arg)
{
	int	ret;

	if (chdir(new_path) == -1)
	{
		info->last_exit_status = ERROR_RET;
		ft_fprintf(STDERR_FILENO, "cd: %s: %s\n", new_path, \
		strerror(errno));
		return (info->last_exit_status);
	}
	if (update_or_add_envp(info, "OLDPWD=", curr_path) == MALLOC_ERROR)
		return (MALLOC_ERROR);
	ret = ft_getcwd(curr_path, PATH_MAX, info);
	if (ret != OK)
	{
		info->last_exit_status = ret;
		return (info->last_exit_status);
	}
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
			info->last_exit_status = ERROR_RET;
			return (ERROR_RET);
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

	new_path = NULL;
	err = ft_getcwd(curr_path, PATH_MAX, info);
	if (err != OK)
		return (err);
	if (args[1] == NULL || ft_strncmp(args[1], "~", sizeof("~")) == 0)
	{
		new_path = find_envp_arg(info->envp, "HOME", ft_strlen("HOME"));
		if (new_path == NULL)
		{
			ft_fprintf(STDERR_FILENO, "cd: HOME not set\n");
			info->last_exit_status = ERROR_RET;
			return (ERROR_RET);
		}
	}
	else if (args[1] != NULL)
	{
		if (ft_cd2(info, args[1], &new_path) != OK)
			return (info->last_exit_status);
	}
	return (ft_chdir(info, new_path, curr_path, args[1]));
}
