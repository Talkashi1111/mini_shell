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

int	pipe_children(t_token_list *start, t_token_list *end, t_node *pipe_node)
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
			pipe_node->child[i] = tree_maker(start, token);
			if (!pipe_node->child[i])
				return (MALLOC_ERROR);
			token = token->next;
			start = token;
			i++;
		}
		else
			token = token->next;
	}
	pipe_node->child[i] = tree_maker(start, end);
	if (!pipe_node->child[i])
		return (MALLOC_ERROR);
	return (OK);
}

int	init_cmd_node(t_token_list *start, t_token_list *end, t_node *cmd_node)
{
	t_token_list	*token;

	while (start != end)
	{
		token = copy_token(start);
		if (!token)
			return (MALLOC_ERROR);
		if (start->type >= STDIN && start->type <= STDOUT_APPEND)
		{
			add_back_token_list(&cmd_node->redi, token);
			start = start->next;
			token = copy_token(start);
			if (!token)
				return (MALLOC_ERROR);
			add_back_token_list(&cmd_node->redi, token);
		}
		else if (start->type == WORD)
			add_back_token_list(&cmd_node->args, token);
		start = start->next;
	}
	return (OK);
}

t_node	*tree_maker(t_token_list *start, t_token_list *end)
{
	t_token_list	*tmp_token;
	unsigned int	pipe_nb;

	tmp_token = search_operator(start, end);
	if (tmp_token)
		return (handle_operator(start, end, tmp_token));
	pipe_nb = count_pipes(start, end);
	if (pipe_nb)
		return (handle_pipe(start, end, pipe_nb));
	if (start->type == OPENPAR)
		return (handle_parenthesis(start));
	return (handle_cmd(start, end));
}
