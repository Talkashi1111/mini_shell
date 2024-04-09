/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:46:06 by achappui          #+#    #+#             */
/*   Updated: 2024/04/02 17:46:06 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "minishell.h"

unsigned int	token_list_size(t_token_list *list)
{
	unsigned int	size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

int	parse_cmd(t_node *node, t_minishell *info)
{
	if (expand_dollars(node->args, info) != OK ||
		expand_dollars(node->redi, info) != OK ||
		wildcard_handler(&node->args, info) != OK ||
		wildcard_handler(&node->redi, info) != OK ||
		remove_quotes(node->args, info) != OK ||
		remove_quotes(node->redi, info) != OK)
		return (info->last_exit_status);
	return (info->last_exit_status);
}

char	**token_list_to_args(t_token_list *node, t_minishell *info)
{
	unsigned int	size;
	unsigned int	i;
	char			**args;

	size = token_list_size(node);
	args = ft_calloc((size + 1), sizeof(char *));
	if (!args)
	{
		info->last_exit_status = MALLOC_ERROR;
		return (NULL);
	}
	i = 0;
	while (node)
	{
		args[i] = ft_strdup(node->str);
		if (!args[i])
		{
			info->last_exit_status = MALLOC_ERROR;
			free_args(args);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (args);
}

int	execute_cmd(t_node *node, t_minishell *info)
{
	char		**args;
	t_pfunc		func;

	if (parse_cmd(node, info) != OK)
		return (info->last_exit_status);
	if (node->args == NULL)
	{
		ft_fprintf(STDERR_FILENO, "command is empty\n");
		return (OK);
	}
	
	func = is_builtin(node->args->str, info->builtins);
    if (func)
	{
		args = token_list_to_args(node->args, info);
		if (info->last_exit_status != OK)
			return (info->last_exit_status);
        info->last_exit_status = func(args, info);
		free_args(args);
	}
    else
	{
		if (execute_non_builtin(node, info) != OK)
        	{
				ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", node->args->str);
				return (info->last_exit_status);
			}
		info->last_exit_status = OK;
	}
	return (info->last_exit_status);
}

int	handle_subshell()//aiko
{
	ft_printf("subshell\n");
	// pid_t	pid;
	// int		status;

	// pid = fork();
	// if (pid == 0)
	// {
	// 	//ya des choses a free ?
	// 	exit(run(node->child[0]));
	// }
	// waitpid(pid, &status, 0);
	// return (WEXITSTATUS(status));
	return (OK);
}

void	ft_child(t_node *node, unsigned int i, t_minishell *info)
{
	if (i > 0)
	{
		if (dup2(info->fd_pipe[i - 1][PIPE_OUT], STDIN_FILENO) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
				info->fd_pipe[i - 1][PIPE_OUT], STDIN_FILENO, strerror(errno));
			ft_free_pipes(node, info);
			exit(info->last_exit_status);
		}
	}
	if (i < node->pipe_nb)
	{
		if (dup2(info->fd_pipe[i][PIPE_IN], STDOUT_FILENO) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
				info->fd_pipe[i][PIPE_IN], STDOUT_FILENO, strerror(errno));
			ft_free_pipes(node, info);
			exit(info->last_exit_status);
		}
	}
	ft_free_pipes(node, info);
	info->last_exit_status = ft_run(node, info);
	ft_exit(NULL, info);
}

int	handle_pipex(t_node *node, t_minishell *info)
{
	unsigned int		i;
	pid_t				pid;

	if (ft_open_pipes(node, info) != OK)
	{
		ft_free_pipes(node, info);
		return (info->last_exit_status);
	}
	i = 0;
	while (i < node->child_nb)
	{
		pid = fork();
		if (pid == 0)
			ft_child(node->child[i], i, info);
		else if (pid < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
			break ;
		}
		i++;
	}
	ft_free_pipes(node, info);
	return (ft_wait_pid(pid, info));
}

int	ft_run(t_node *node, t_minishell *info)
{
	int	ret;

	if (node->type == AND)
	{
		ret = ft_run(node->child[0], info);
		if (ret == OK)
			return (ft_run(node->child[1], info));
		return (ret);
	}
	else if (node->type == OR)
	{
		ret = ft_run(node->child[0], info);
		if (ret != OK)
			return (ft_run(node->child[1], info));
		return (ret);
	}
	else if (node->type == PIPE)
		return (handle_pipex(node, info));
	else if (node->type == OPENPAR)
		return (handle_subshell());
	else
		return (execute_cmd(node, info));
}