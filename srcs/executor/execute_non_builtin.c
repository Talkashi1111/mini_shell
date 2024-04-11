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

char *match_path(char *cmd, t_minishell *info)
{
	char 	*origin_path_env;
	char	**splitted_path;
	int i;
	char *path;
	
	if (!cmd || cmd[0] == '/' || !ft_strncmp(cmd, "./", 2) || 
		!ft_strncmp(cmd, "../", 3))
		return (cmd);
	
	origin_path_env = find_envp_arg(info->envp, "PATH", 4);
	if (!origin_path_env)
		return (cmd);//how to return
	splitted_path = ft_split(origin_path_env + 5, ':');
	if (!splitted_path)
	{
		ft_fprintf(STDERR_FILENO, "failed to split the env paths\n");
		info->last_exit_status = MALLOC_ERROR;
		//free everything because we are in child process
		return (NULL);
	}
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
	return (cmd);
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

int execute_non_builtin(t_node *node, t_minishell *info)
{
	pid_t 	pid;
	char **args;
	char *path;
	int 	size;
	int 	i;

	pid = fork();
	if (pid == 0)
	{
		path = match_path(node->args->str, info);
		if (!path)
		{
			info->last_exit_status = MALLOC_ERROR;
			ft_exit(NULL, info);
		}
		size = token_list_size(node->args);
		args = ft_calloc((size + 1), sizeof(char *));
		if (!args)
		{
			free(path);
			info->last_exit_status = MALLOC_ERROR;
			ft_exit(NULL, info);
		}
		args[0] = path;
		node->args = node->args->next;
		i = 1;
		while(node->args)
		{
			args[i] = ft_strdup(node->args->str);
			if (!args[i])
			{
				info->last_exit_status = MALLOC_ERROR;
				ft_exit(args, info);
			}
			node->args = node->args->next;
			i++;
		}
		free_tokens_and_tree(info);
		execve(path, args, info->envp);
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "execve: %s\n", strerror(errno));
		ft_exit(args, info);
	}
	else if (pid < 0)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	return (ft_wait_pid(pid, info));
}