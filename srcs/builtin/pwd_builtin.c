/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:45:30 by achappui          #+#    #+#             */
/*   Updated: 2024/05/22 11:04:59 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_getcwd(char *path, size_t size, t_minishell *info)
{
	if (getcwd(path, size) == NULL)
	{
		info->last_exit_status = ERROR_RET;
		ft_fprintf(STDERR_FILENO, "pwd: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	return (OK);
}

int	ft_pwd(char *args[], t_minishell *info)
{
	char	path[PATH_MAX];
	int		err;
	char	*pwd;

	(void)args;
	pwd = find_envp_arg(info->envp, "PWD", ft_strlen("PWD"));
	if (pwd)
	{
		ft_printf("%s\n", pwd);
		return (OK);
	}
	err = ft_getcwd(path, sizeof(path), info);
	if (err != OK)
		return (info->last_exit_status);
	err = update_or_add_envp(info, "PWD=", path);
	if (err != OK)
		return (err);
	ft_printf("%s\n", path);
	return (OK);
}
