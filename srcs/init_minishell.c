/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 09:35:17 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 13:15:46 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_init(t_minishell *info)
{
	info->builtins[0] = (t_builtin){"cd", ft_cd};
	info->builtins[1] = (t_builtin){"echo", ft_echo};
	info->builtins[2] = (t_builtin){"env", ft_env};
	info->builtins[3] = (t_builtin){"exit", ft_exit};
	info->builtins[4] = (t_builtin){"export", ft_export};
	info->builtins[5] = (t_builtin){"pwd", ft_pwd};
	info->builtins[6] = (t_builtin){"unset", ft_unset};
	info->builtins[7] = (t_builtin){NULL, NULL};
}

int	minishell_init(t_minishell *info, char **envp)
{
	if (save_std_streams(info) != OK)
		return (info->last_exit_status);
	info->envp = copy_env(envp);
	if (!info->envp || update_or_add_envp(info, "OLDPWD=", "") != OK)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "bash: malloc: %s\n",
			strerror(errno));
		return (info->last_exit_status);
	}
	info->last_exit_status = OK;
	info->token_list = NULL;
	info->tree = NULL;
	builtin_init(info);
	info->fd_pipe = NULL;
	info->heredocs_list = NULL;
	info->pipe_nb = 0;
	return (OK);
}

void	no_line(t_minishell *info, char *line)
{
	if (!line)
	{
		info->last_exit_status = OK;
		ft_printf("exit\n");
		ft_exit(NULL, info);
	}
}
