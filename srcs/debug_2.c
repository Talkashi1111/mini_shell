/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 19:12:20 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 19:16:02 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_token_list(t_token_list *token)
{
	if (!token)
	{
		ft_printf("empty token list\n");
		return ;
	}
	while (token)
	{
		ft_printf("\"%s\" TYPE: %i ", token->str, token->type);
		token = token->next;
	}
	ft_printf("\n");
}

void	display_2(t_node *node, int depth, int *siblings)
{
	int	i;

	ft_printf("CMD\n");
	i = 0;
	while (i < depth)
	{
		if (siblings[i])
			ft_printf("│   ");
		else
			ft_printf("    ");
		i++;
	}
	ft_printf("  args: ");
	display_token_list(node->args);
	i = 0;
	while (i < depth)
	{
		if (siblings[i])
			ft_printf("│   ");
		else
			ft_printf("    ");
		i++;
	}
	ft_printf("  redi: ");
	display_token_list(node->redi);
}
