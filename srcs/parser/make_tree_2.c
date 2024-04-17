/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_tree_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:13:43 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 19:13:43 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_back_token_list(t_token_list **list, t_token_list *token)
{
	t_token_list	*index;

	if (!*list)
		*list = token;
	else
	{
		index = *list;
		while (index->next)
			index = index->next;
		index->next = token;
	}
}

t_token_list	*skip_parenthesis(t_token_list *token)
{
	unsigned int	scope;

	scope = 0;
	while (TRUE)
	{
		if (token->type == OPENPAR)
			scope++;
		else if (token->type == CLOSEPAR && --scope == 0)
			return (token);
		token = token->next;
	}
}

t_node	*new_tree_node(char type, unsigned int child_nb, unsigned int pipe_nb)
{
	t_node	*new_node;

	new_node = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!new_node)
	{
		ft_fprintf(STDERR_FILENO, "failed to allocate tree node [type=%d]: %s\n", type, strerror(errno));
		return (NULL);
	}
	new_node->type = type;
	new_node->child_nb = child_nb;
	new_node->pipe_nb = pipe_nb;
	if (child_nb)
	{
		new_node->child = (t_node **)ft_calloc(child_nb, sizeof(t_node *));
		if (!new_node->child)
		{
			ft_fprintf(STDERR_FILENO, "failed to allocate children array for tree node [type=%d]: %s\n", type, strerror(errno));
			free(new_node);
			return (NULL);
		}
	}
	return (new_node);
}

void free_tree(t_node *node)
{
	unsigned int	i;
	
	if (!node)
		return ;
	if (node->child_nb)
	{
		i = 0;
		while (i < node->child_nb && node->child[i])
		{
			free_tree(node->child[i]);
			node->child[i] = NULL;
			i++;
		}
		free(node->child);
		node->child = NULL;
	}
	free_token_list(node->redi);
	free_token_list(node->args);
	free(node);
}