/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:57:47 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 13:43:47 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	parse_cmd(t_node *node, t_minishell *info)
{
	if (expand_dollars(&node->args, info, 1) != OK ||
		expand_dollars(&node->redi, info, 1) != OK ||
		wildcard_handler(&node->args, info, 1) != OK ||
		wildcard_handler(&node->redi, info, 1) != OK ||
		remove_quotes(node->args, info, 1) != OK ||
		remove_quotes(node->redi, info, 1) != OK)
		return (info->last_exit_status);
	return (OK);
}

 //a voir globalement pour la gestion d'erreur, si ici je return errno alors que on entre justement dans cette fonction pour quitter le programme
 //ca pourrait fausser la valeur de retour


int	pre_execution(t_node *node, char ***args, t_minishell *info, t_pfunc *func)
{
	if (parse_cmd(node, info) != OK)//i don't understand , in this case we cannot parse the $? from last command...
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
	t_pfunc		func;

	args = NULL;
	func = NULL;
	if (pre_execution(node, &args, info, &func) == OK)
	{
		info->last_exit_status = apply_redirections(node->redi, info);
		if (info->last_exit_status == OK)
		{
			if (func)
				info->last_exit_status = func(args, info);
			else if (args[0])
				info->last_exit_status = execute_non_builtin(args, info);
		}
	}
	free_args(args);
	restore_std_streams(info->saved_streams, info);
	return (info->last_exit_status);
}
