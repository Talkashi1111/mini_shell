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

#include "minishell.h"

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
		return (handle_command(node, info));
}
