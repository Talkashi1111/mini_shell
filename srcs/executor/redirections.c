/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 21:32:26 by achappui          #+#    #+#             */
/*   Updated: 2024/04/11 16:33:38 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>


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
		ft_printf(STDERR_FILENO, "bash: ambigous redirection\n");
	return (info->last_exit_status);
}

int	restore_redirection()
{
	
}

int	apply_redirections(t_node *node, t_minishell *info)
{
	if (check_redirections(node, info) != OK)
		return (info->last_exit_status);
	
	
}