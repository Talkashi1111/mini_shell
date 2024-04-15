/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_non_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:04:15 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/15 23:25:44 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char	*ft_form_path(char *path, char *cmd)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		ft_fprintf(STDERR_FILENO, "failed alloc path '%s/': %s\n",
			path, strerror(errno));
		return (NULL);
	}
	ret = ft_strjoin(tmp, cmd);
	if (!ret)
	{
		ft_fprintf(STDERR_FILENO, "failed alloc path '%s%s': %s\n",
			tmp, cmd, strerror(errno));
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (ret);
}

char *ft_search_in_paths(char **splitted_path, char *cmd)
{
	int i;
	char *path;

	i = 0;
	while (splitted_path[i])
	{
		path = ft_form_path(splitted_path[i], cmd);
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
		ft_fprintf(STDERR_FILENO, "failed to split PATH\n");
		info->last_exit_status = MALLOC_ERROR;
		return (NULL);
	}
	return (ft_search_in_paths(splitted_path, cmd));
}


int ft_wait_pid(int child_pid, t_minishell *info)
{
	pid_t pid_tmp;
	int status;

	while (TRUE)
	{
		pid_tmp = waitpid(-1, &status, 0);
		if (pid_tmp == -1) {
			if (errno == ECHILD)
				break ;
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
			break ;
		}
		if (pid_tmp != child_pid)
			continue ;
		if (WIFEXITED(status))
			info->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			info->last_exit_status = 128 + WTERMSIG(status);
		else
			ft_fprintf(STDERR_FILENO, "Child process exited with unknown status\n");
	}
	return (info->last_exit_status);
}

int	command_child_process(t_node *node, t_minishell *info, int heredoc_pipe[2])
{
	char	**args;
	char	*path;

	if (dup2(heredoc_pipe[0], 0) == -1)
	{
		close(heredoc_pipe[0]);
		close(heredoc_pipe[1]);
		ft_exit(NULL, info);
	}
	close(heredoc_pipe[0]);
	close(heredoc_pipe[1]);
	if (apply_redirections(node, info, heredoc_pipe) != OK)
		ft_exit(NULL, info);
	args = token_list_to_args(node->args, info);
	if (!args)
		ft_exit(NULL, info);
	path = match_path(node->args->str, info);
	if (!path)
	{
		info->last_exit_status = MALLOC_ERROR;
		ft_exit(args, info);
	}
	free(args[0]);
	args[0] = path;
	execve(args[0], args, info->envp);
	info->last_exit_status = errno;
	ft_fprintf(STDERR_FILENO, "execve: %s\n", strerror(errno));
	ft_exit(args, info);
	return (info->last_exit_status);
}

int execute_non_builtin(t_node *node, t_minishell *info, int heredoc_pipe[2])
{
    pid_t     pid;

    pid = fork();
    if (pid == 0)
    {
        command_child_process(node, info, heredoc_pipe);
    }
    else if (pid < 0)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    return (ft_wait_pid(pid, info));
}
