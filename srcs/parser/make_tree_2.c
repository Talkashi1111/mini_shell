/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_tree_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:12:51 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 19:12:51 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*handle_operator(t_token_list *start, t_token_list *end, t_token_list *tmp_token)
{
	t_node			*new_node;

	new_node = new_tree_node(tmp_token->type, 2, 0);
	if (!new_node)
		return (NULL);
	new_node->child[0] = tree_maker(start, tmp_token);
	if (!new_node->child[0])
	{
		free_tree(new_node);
		return (NULL);
	}
	new_node->child[1] = tree_maker(tmp_token->next, end);
	if (!new_node->child[1])
	{
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

t_node	*handle_pipe(t_token_list *start, t_token_list *end, unsigned int	pipe_nb)
{
	t_node			*new_node;

	new_node = new_tree_node(PIPE, pipe_nb + 1, pipe_nb);
	if (!new_node)
		return (NULL);
	if (pipe_children(start, end, new_node) != OK)
	{
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

t_node	*handle_parenthesis(t_token_list *start)
{
	t_node			*new_node;

	new_node = new_tree_node(OPENPAR, 1, 0);
	if (!new_node)
		return (NULL);
	new_node->child[0] = tree_maker(start->next, skip_parenthesis(start));
	if (!new_node->child[0])
	{
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

t_node	*handle_cmd(t_token_list *start, t_token_list *end)
{
	t_node			*new_node;

	new_node = new_tree_node(CMD, 0, 0);
	if (!new_node)
		return (NULL);
	if (init_cmd_node(start, end, new_node) != OK)
	{
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}
