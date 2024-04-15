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
#include <fcntl.h>
#include "minishell.h"
#include <stdio.h>

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
	if (expand_dollars(&node->args, info) != OK ||
		expand_dollars(&node->redi, info) != OK ||
		wildcard_handler(&node->args, info) != OK ||
		wildcard_handler(&node->redi, info) != OK ||
		remove_quotes(node->args, info) != OK ||
		remove_quotes(node->redi, info) != OK)
		return (info->last_exit_status);
	return (OK);
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

int    save_stdstreams(int stdstreams[2], t_minishell *info)
{
    stdstreams[0] = dup(0);
    if (stdstreams[0] == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    stdstreams[1] = dup(1);
    if (stdstreams[1] == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup: %s\n", strerror(errno));
        return (info->last_exit_status);    
    }
    return (OK);
}

int    restore_stdstreams(int stdstreams[2], t_minishell *info)
{
    if (dup2(stdstreams[0], 0) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
        return (info->last_exit_status);    
    }
    if (dup2(stdstreams[1], 1) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
        return (info->last_exit_status);    
    }
    if (close(stdstreams[0]) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
    }
    if (close(stdstreams[1]) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
    }
    return (OK);
}

int	check_redirections(t_node *node, t_minishell *info)
{
	t_token_list	*index;

	index = node->redi;
	info->last_exit_status = OK;
	if (index)
	{
		while (index->next)
		{
			if (((index->type >= STDIN && index->type <= STDOUT_APPEND) && \
			(index->next->type >= STDIN && index->next->type <= STDOUT_APPEND)) ||
			(index->type == WORD && index->next->type == WORD))
				info->last_exit_status = 1;
			index = index->next;	
		}
		if (index->type >= STDIN && index->type <= STDOUT_APPEND)
			info->last_exit_status = 1;
	}
	if (info->last_exit_status != OK)
		ft_fprintf(STDERR_FILENO, "bash: ambigous redirection\n");
	return (info->last_exit_status);
}

void	get_last_stdin_stdout(t_token_list *redi, t_token_list **last_stdin, t_token_list **last_stdout)
{
	while (redi)
	{
		if (redi->type == STDIN || redi->type == STDIN_HEREDOC)
			*last_stdin = redi;
		else if (redi->type == STDOUT || redi->type == STDOUT_APPEND)
			*last_stdout = redi;
		redi = redi->next;
		redi = redi->next;
	}
}

int	apply_redirections(t_node *node, t_minishell *info, int heredoc_pipe[2]) //checker les erreurs
{
	int				fd;
	t_token_list	*last_stdin;
	t_token_list	*last_stdout;

	(void)info; //a utiliser pour la gestion d'erreur, alors enlever le void
	get_last_stdin_stdout(node->redi, &last_stdin, &last_stdout);
	if (last_stdin && last_stdin->type == STDIN)
	{
		fd = open(last_stdin->next->str, O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if (last_stdin && last_stdin->type == STDIN_HEREDOC)
	{
		dup2(heredoc_pipe[0], 0);
		close(heredoc_pipe[0]);
		close(heredoc_pipe[1]);
	}
	if (last_stdout && last_stdout->type == STDOUT)
	{
		fd = open(last_stdout->next->str, O_WRONLY);
		dup2(fd, 1);
		close(fd);
	}
	else if (last_stdout && last_stdout->type == STDOUT_APPEND)
	{
		fd = open(last_stdout->next->str, O_WRONLY | O_APPEND);
		dup2(fd, 1);
		close(fd);
	}

	return (OK);
}

t_token_list *get_last_heredoc(t_token_list *redi)
{
	t_token_list	*last_heredoc;

	last_heredoc = NULL;
	while (redi)
	{
		if (redi->type == STDIN_HEREDOC)
			last_heredoc = redi;
		redi = redi->next;
		redi = redi->next;
	}
	return (last_heredoc);
}

int	run_heredocs(t_node *node, t_minishell *info, int heredoc_pipe[2], int stdstreams[2])
{
	pid_t			pid;
	t_token_list	*index;
	t_token_list	*last_heredoc;
	char			*line;

	pipe(heredoc_pipe);
	pid = fork();
	if (pid == 0)
	{
		last_heredoc = get_last_heredoc(node->redi);
		index = node->redi;
		dup2(heredoc_pipe[1], 1);
		close(heredoc_pipe[0]);
		close(stdstreams[0]);
		close(stdstreams[1]);
		while (index)
		{
			if (index->type == STDIN_HEREDOC && index != last_heredoc)
			{
				line = get_next_line(0);

				while (ft_strncmp(line, index->next->str, ft_strlen(line)) != 0)
				{
					free(line);
					line = get_next_line(0);
				}
				free(line);
			}
			else if (index->type == STDIN_HEREDOC && index == last_heredoc)
			{
				line = get_next_line(0);

				while (ft_strncmp(line, index->next->str, ft_strlen(line)) != 0)
				{
					write(1, line, ft_strlen(line));
					free(line);
					line = get_next_line(0);
				}
				free(line);
			}
			index = index->next;
			index = index->next;
		}

		ft_exit(NULL, info);
	}
	return (pid);
}

int	execute_cmd(t_node *node, t_minishell *info)
{
	pid_t		pid;
	char		**args;
	int			heredoc_pipe[2];
	int			stdstreams[2];
	t_pfunc		func;

	pid = -1;
	stdstreams[0] = -1;
	stdstreams[1] = -1;
	heredoc_pipe[0] = -1;
	heredoc_pipe[1] = -1;
	info->last_exit_status = OK;
	if (parse_cmd(node, info) != OK)
		return (info->last_exit_status);
	if (node->args == NULL && node->redi == NULL) //modified
		return (OK);
	if (check_redirections(node, info) != OK)
		return (info->last_exit_status);
	func = is_builtin(node->args->str, info->builtins);
    if (func)
	{
		if (node->redi)
			if (save_stdstreams(stdstreams, info) != OK)
				return (info->last_exit_status);
		pid = run_heredocs(node, info, heredoc_pipe, NULL);
		if (pid != OK)
		{
			if (heredoc_pipe[0] != -1)
				close(heredoc_pipe[0]);
			if (heredoc_pipe[1] != -1)
				close(heredoc_pipe[1]);
			if (stdstreams[0] != -1)
				close(stdstreams[0]);
			if (stdstreams[1] != -1)
				close(stdstreams[1]);
			return (info->last_exit_status);
		}
		if (apply_redirections(node, info, heredoc_pipe) != OK)
			printf("ERREUR A GERER\n"); //a gerer cette erreur
		args = token_list_to_args(node->args, info);
		if (info->last_exit_status != OK)
		{
			if (heredoc_pipe[0] != -1)
				close(heredoc_pipe[0]);
			if (heredoc_pipe[1] != -1)
				close(heredoc_pipe[1]);
			if (stdstreams[0] != -1)
				close(stdstreams[0]);
			if (stdstreams[1] != -1)
				close(stdstreams[1]);
			return (info->last_exit_status);
		}
        info->last_exit_status = func(args, info);
		free_args(args);
		if (heredoc_pipe[0] != -1)
			close(heredoc_pipe[0]);
		if (heredoc_pipe[1] != -1)
			close(heredoc_pipe[1]);
		if (node->redi)
			if (restore_stdstreams(stdstreams, info) != OK)
				return (info->last_exit_status);
	}
    else
	{
		pid = run_heredocs(node, info, heredoc_pipe, NULL);
		if (pid != OK)
		{
			if (heredoc_pipe[0] != -1)
				close(heredoc_pipe[0]);
			if (heredoc_pipe[1] != -1)
				close(heredoc_pipe[1]);
			return (info->last_exit_status);
		}
		if (execute_non_builtin(node, info, heredoc_pipe) != OK)
		{
			if (heredoc_pipe[0] != -1)
				close(heredoc_pipe[0]);
			if (heredoc_pipe[1] != -1)
				close(heredoc_pipe[1]);
			return (info->last_exit_status);
		}
		if (heredoc_pipe[0] != -1)
			close(heredoc_pipe[0]);
		if (heredoc_pipe[1] != -1)
			close(heredoc_pipe[1]);
		info->last_exit_status = OK;
	}
	if (pid != -1)
	{
		waitpid(pid, NULL, 0); //recuperer le status
	}
	return (info->last_exit_status);
}

int	handle_subshell(t_node *node, t_minishell *info)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (pid == 0)
	{
		info->last_exit_status = ft_run(node->child[0], info);
		ft_exit(NULL, info);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (WIFEXITED(status))
		info->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		info->last_exit_status = 128 + WTERMSIG(status);
	else
		ft_fprintf(STDERR_FILENO, "Child process exited with unknown status\n");
	return (info->last_exit_status);
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
			ft_free_pipes(info);
			exit(info->last_exit_status);
		}
	}
	if (i < info->pipe_nb)
	{
		if (dup2(info->fd_pipe[i][PIPE_IN], STDOUT_FILENO) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
				info->fd_pipe[i][PIPE_IN], STDOUT_FILENO, strerror(errno));
			ft_free_pipes(info);
			exit(info->last_exit_status);
		}
	}
	ft_free_pipes(info);
	info->last_exit_status = ft_run(node, info);
	ft_exit(NULL, info);
}

int	handle_pipex(t_node *node, t_minishell *info)
{
	unsigned int		i;
	pid_t				pid;

	if (ft_open_pipes(node, info) != OK)
	{
		ft_free_pipes(info);
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
	ft_free_pipes(info);
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
		return (handle_subshell(node, info));
	else
		return (execute_cmd(node, info));
}