/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_maker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:46:31 by achappui          #+#    #+#             */
/*   Updated: 2024/04/02 17:46:31 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*search_operator(t_token_list *start, t_token_list *end)//end est l'element juste apres le dernier, ca peut etre NULL
{
	t_token_list	*token;
	t_token_list	*saved_token;

	saved_token = NULL;
	token = start;
	while (token != end)
	{
		if (token->type == OPENPAR)
			token = skip_parenthesis(token);
		else if (token->type == OR || token->type == AND)
			saved_token = token;
		token = token->next;
	}
	return (saved_token);
}

unsigned int	count_pipes(t_token_list *start, t_token_list *end)
{
	unsigned int	pipe_nb;

	pipe_nb = 0;
	while (start != end)
	{
		if (start->type == OPENPAR)
			start = skip_parenthesis(start);
		else if (start->type == PIPE)
			pipe_nb++;
		start = start->next;
	}
	return (pipe_nb);
}

int	pipe_children(t_token_list *start, t_token_list *end, t_node *pipe_node, t_minishell *info)
{
	t_token_list	*token;
	int				i;

	token = start;
	i = 0;
	while (token != end)
	{
		if (token->type == OPENPAR)
			token = skip_parenthesis(token);
		else if (token->type == PIPE)
		{
			pipe_node->child[i] = tree_maker(start, token, info);
			if (!pipe_node->child[i])
				return (MALLOC_ERROR);
			token = token->next;
			start = token;
			i++;
		}
		else
			token = token->next;
	}
	pipe_node->child[i] = tree_maker(start, end, info);
	if (!pipe_node->child[i])
		return (MALLOC_ERROR);
	return (OK);
}

char	*ft_itoa_heredoc(uintptr_t n)
{
	uintptr_t	tab;
	short		i;

	tab = n;
	i = 2;
	while ((tab >= 10) && i++)
		tab /= 10;
	tab = (long long)malloc((i + 2) * sizeof(char));
	if (!tab)
		return (NULL);
	((char *)tab)[i] = '\0';
	((char *)tab)[0] = '.';
	while (n > 0 || (i == 1))
	{
		((char *)tab)[--i] = n % 10 + 48;
		n /= 10;
	}
	return ((char *)tab);
}

int	heredoc_expander(uintptr_t file_no, char **eof, t_minishell *info)
{
	char			*name;
	char			*line;
	unsigned int	eof_len;
	int				fd;

	name = ft_itoa_heredoc(file_no);
	if (!name)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "malloc: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0777); //pour les perm autre chose ?
	if (fd == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
		free(name);
		return (info->last_exit_status);
	}
	eof_len = ft_strlen(*eof);
	line = readline(">");
	if (!line)
	{
		if (close(fd) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
		}
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "read: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	write(fd, line, ft_strlen(line)); //checker le write
	while (ft_strncmp(line, *eof, eof_len) != 0)
	{
		free(line);
		line = readline(">");
		if (!line)
		{
			if (close(fd) == -1)
			{
				info->last_exit_status = errno;
				ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
			}
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "read: %s\n", strerror(errno));
			return (info->last_exit_status);
		}
		write(fd, line, ft_strlen(line)); //checker le write
	}
	free(line);
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	free(*eof);
	*eof = name; //erreur mal geree a coriger a cause du remplacement de eof par name
	return (OK);
}

int	init_cmd_node_redi(t_token_list *start, t_node *cmd_node, t_token_list *token_1, t_minishell *info)
{
	t_token_list	*token_2;

	add_back_token_list(&cmd_node->redi, token_1);
	token_2 = copy_token(start->next, info);
	if (!token_2)
		return (info->last_exit_status);
	if (token_1->type == STDIN_HEREDOC)
	{
		if (heredoc_expander((uintptr_t)token_2, &token_2->str, info) != OK)
		{
			free(token_2->str);
			free(token_2);
			return (info->last_exit_status);
		}
	}
	add_back_token_list(&cmd_node->redi, token_2);
	return (OK);
}

int	init_cmd_node(t_token_list *start, t_token_list *end, t_node *cmd_node, t_minishell *info)
{
	t_token_list	*token;

	while (start != end)
	{
		token = copy_token(start, info);
		if (!token)
			return (info->last_exit_status);
		if (start->type >= STDIN && start->type <= STDOUT_APPEND)
		{
			if (init_cmd_node_redi(start, cmd_node, token, info) != OK)
				return (info->last_exit_status);
			start = start->next;
		}
		else if (start->type == WORD)
			add_back_token_list(&cmd_node->args, token);
		start = start->next;
	}
	return (OK);
}

t_node	*tree_maker(t_token_list *start, t_token_list *end, t_minishell *info)
{
	t_token_list	*tmp_token;
	unsigned int	pipe_nb;

	tmp_token = search_operator(start, end);
	if (tmp_token)
		return (handle_operator(start, end, tmp_token, info));
	pipe_nb = count_pipes(start, end);
	if (pipe_nb)
		return (handle_pipe(start, end, pipe_nb, info));
	if (start->type == OPENPAR)
		return (handle_parenthesis(start, info));
	return (handle_cmd(start, end, info));
}
