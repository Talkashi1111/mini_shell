/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:57:47 by achappui          #+#    #+#             */
/*   Updated: 2024/04/17 00:19:29 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

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

 //a voir globalement pour la gestion d'erreur, si ici je return errno alors que on entre justement dans cette fonction pour quitter le programme
 //ca pourrait fausser la valeur de retour


int	pre_execution(t_node *node, char ***args, t_minishell *info, t_pfunc *func)
{
	if (parse_cmd(node, info) != OK)
		return (info->last_exit_status);
	*args = token_list_to_args(node->args, info);
	if (!args)
		return (info->last_exit_status);
	if (check_redirections(node, info) != OK)
		return (info->last_exit_status);
	*func = is_builtin((*args)[0], info->builtins);
	return (OK);
}



int	handle_command(t_node *node, t_minishell *info)
{
	char		**args;
	int			saved_streams[2];
	t_pfunc		func;

	saved_streams[0] = -1;
	saved_streams[1] = -1;
	args = NULL;
	func = NULL;
	info->last_exit_status = OK;
	if (pre_execution(node, &args, info, &func) == OK && save_std_streams(saved_streams, info) == OK)
	{
		if (apply_redirections(saved_streams, node->redi, info) == OK)
		{
			if (func)
				info->last_exit_status = func(args, info);
			else if (args[0])
				info->last_exit_status = execute_non_builtin(args, saved_streams, info);
		}
	}
	free_args(args);
	restore_std_streams(saved_streams, info);
	return (info->last_exit_status);
}