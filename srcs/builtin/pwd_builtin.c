/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:45:30 by achappui          #+#    #+#             */
/*   Updated: 2024/04/02 17:48:51 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_getcwd(char *path, size_t size)
{
	int err;

	if (getcwd(path, size) == NULL)
	{
		err = errno;
		ft_fprintf(STDERR_FILENO, "pwd: %s\n", strerror(err));
		return (err);
	}
	return (OK);
}

int ft_pwd(char *args[], char **envp[])
{
	char	path[PATH_MAX];
	int		err;
	char	*pwd;

	if (args[1])
	{
		ft_fprintf(STDERR_FILENO, "pwd: too many arguments\n");
		return (USAGE_ERROR);
	}
	pwd = find_envp_arg(*envp, "PWD", 0);
	if (pwd)
	{
		ft_printf("%s\n", pwd);
		return (OK);
	}
	err = ft_getcwd(path, sizeof(path));
	if (err != OK)
		return (err);
	err = update_or_add_envp(envp, "PWD=", path);
	if (err != OK)
		return (err);
	ft_printf("%s\n", path);
	return (OK);
}
