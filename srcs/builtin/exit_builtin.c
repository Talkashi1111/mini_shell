/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:19:55 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/19 21:37:23 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char *args[], t_minishell *info)
{
	free_args(args);
	free_tree(info->tree);
	free_token_list(info->token_list);
	free_token_list(info->heredocs_list);
	//free_token_list_and_heredocs(info->token_list, info);//we need to free heredoc here?
	free_args(info->envp);
	rl_clear_history();
	ft_close_fds(info->saved_streams, info);
	exit(info->last_exit_status);
	return (OK);
}
