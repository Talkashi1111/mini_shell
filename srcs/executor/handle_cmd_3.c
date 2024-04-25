/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:04:15 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 13:17:00 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_form_path(char *path, char *cmd, t_minishell *info)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		ft_fprintf(info->saved_streams[1], "failed alloc path '%s/': %s\n",
			path, strerror(errno));
		return (NULL);
	}
	ret = ft_strjoin(tmp, cmd);
	if (!ret)
	{
		ft_fprintf(info->saved_streams[1], "failed alloc path '%s%s': %s\n",
			tmp, cmd, strerror(errno));
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (ret);
}

char *ft_search_in_paths(char **splitted_path, char *cmd, t_minishell *info)
{
	int i;
	char *path;

	i = 0;
	while (splitted_path[i])
	{
		path = ft_form_path(splitted_path[i], cmd, info);
		if (path && access(path, X_OK) == 0)
		{
			free_args(splitted_path);
			return (path);
		}
		free(path);
		i++;
	}
	free_args(splitted_path);
	return (ft_strdup(cmd));
}

char *match_path(char *cmd, t_minishell *info)
{
	char 	*path_value;
	char	**splitted_path;

	if (!cmd || cmd[0] == '/' || !ft_strncmp(cmd, "./", 2) ||
		!ft_strncmp(cmd, "../", 3))
		return (ft_strdup(cmd));
	path_value = find_envp_arg(info->envp, "PATH", 4);
	if (!path_value)
		return (ft_strdup(cmd));
	splitted_path = ft_split(path_value + 5, ':');
	if (!splitted_path)
	{
		ft_fprintf(info->saved_streams[1], "failed to split PATH\n");
		info->last_exit_status = MALLOC_ERROR;
		return (NULL);
	}
	return (ft_search_in_paths(splitted_path, cmd, info));
}

int	command_child_process(char **args, t_minishell *info)
{
	char	*path;

	path = match_path(args[0], info);
	if (!path)
	{
		info->last_exit_status = MALLOC_ERROR;
		free_args(args);
		ft_exit(NULL, info);
	}
	free(args[0]);
	args[0] = path;
	execve(args[0], args, info->envp);
	info->last_exit_status = errno;
	if (errno == ENOENT)
		ft_fprintf(info->saved_streams[1], "%s: command not found\n", args[0]);
	else
		ft_fprintf(info->saved_streams[1], "execve: %s\n", strerror(errno));
	free_args(args);
	ft_exit(NULL, info);
	return (info->last_exit_status);
}

int execute_non_builtin(char **args, t_minishell *info)
{
    pid_t     pid;

    pid = fork();
    if (pid == 0)
        command_child_process(args, info);
    else if (pid < 0)
    {
        info->last_exit_status = errno;
        ft_fprintf(info->saved_streams[1], "fork: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    return (ft_wait_pid(pid, info));
}
