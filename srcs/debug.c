/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:52:30 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 19:17:07 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_3(t_node *node, int depth, int *siblings);

void	display_indentation(int depth, int *siblings)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		if (siblings[i])
			ft_printf("│   ");
		else
			ft_printf("    ");
		i++;
	}
}

void	display_tree_recursive(t_node *node, int depth, int isLast,
	int *siblings)
{
	if (!node)
		return ;
	display_indentation(depth, siblings);
	if (depth > 0)
	{
		if (isLast)
			ft_printf("└── ");
		else
			ft_printf("├── ");
	}
	if (node->type == CMD)
		display_2(node, depth, siblings);
	else
		ft_printf("%s\n", node_type_to_string(node->type));
	if (depth > 0)
		siblings[depth - 1] = !isLast;
	display_3(node, depth, siblings);
}

void	display_3(t_node *node, int depth, int *siblings)
{
	unsigned int	j;

	j = 0;
	while (j < node->child_nb)
	{
		display_tree_recursive(node->child[j], depth + 1,
			j == node->child_nb - 1, siblings);
		j++;
	}
	if (depth > 0)
		siblings[depth - 1] = 0;
}

void	display_tree(t_node *node)
{
	int	siblings[128];
	int	i;

	i = 0;
	while (i < 128)
	{
		siblings[i] = 0;
		i++;
	}
	display_tree_recursive(node, 0, 1, siblings);
}
