/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:57:47 by achappui          #+#    #+#             */
/*   Updated: 2024/04/16 21:56:32 by achappui         ###   ########.fr       */
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


int	pre_execution(t_node *node, t_minishell *info, t_cmd *cmd)
{
	reset_fd_tabs(cmd->heredoc_pipe, cmd->saved_std);
	cmd->args = NULL;
	cmd->func = NULL;
	cmd->pid = -1;
	if (parse_cmd(node, info) != OK)
		return (info->last_exit_status);
	cmd->args = token_list_to_args(node->args, info);
	if (!cmd->args)
		return (info->last_exit_status);
	if (check_redirections(node, info) != OK)
		return (info->last_exit_status);
	cmd->func = is_builtin(cmd->args[0], info->builtins);
	return (OK);
}

int	execute_others(t_cmd *cmd, t_minishell *info, t_node *node)
{
	if (save_std_streams(cmd->saved_std, info) != OK)
		return (info->last_exit_status);
	if (apply_redirections(cmd, info, node->redi) != OK)
	{
		ft_close_size_2(cmd->saved_std, info);
		return (info->last_exit_status);
	}
	if (cmd->func)
		info->last_exit_status = cmd->func(cmd->args, info);
	if (restore_std_streams(cmd->saved_std, info) != OK)
	{
		ft_close_size_2(cmd->saved_std, info);
		return (info->last_exit_status);
	}
	ft_close_size_2(cmd->saved_std, info);
	return (OK);
}
#include <sys/types.h>
#include <sys/wait.h>

int	handle_command(t_node *node, t_minishell *info)
{
	t_cmd	cmd;

	info->last_exit_status = OK;
	if (pre_execution(node, info, &cmd) == OK)
	{
		if (run_potential_heredocs(node->redi, info, &cmd) == OK)
		{
			close(cmd.heredoc_pipe[1]);
			cmd.heredoc_pipe[1] = -1;
			if (cmd.func || !cmd.args[0])
				execute_others(&cmd, info, node);
			else
				execute_non_builtin(&cmd, info, node);
			close(cmd.heredoc_pipe[0]);
			cmd.heredoc_pipe[0] = -1;
		}
	}
	free_args(cmd.args);
	wait_potential_heredocs(cmd.pid, info); //on ne recupere pas la valeur de retour de 
	return (info->last_exit_status);
}