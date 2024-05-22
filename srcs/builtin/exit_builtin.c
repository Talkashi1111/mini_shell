/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:19:55 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/22 11:45:26 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	// ft_printf("bash: exit: %s: numeric argument required", args[0]);
	// info->last_exit_status = 2;

void	handle_args(char *arg, t_minishell *info)
{
	long long			s_l;
	unsigned long long	u_ll;

	if (arg[0] == '-')
		handle_signed();
	else if ()
		handle_unsigned();
	else
		just_print_error();
}

int	ft_exit(char *args[], t_minishell *info)
{
	if (args)
		handle_args(args[0], info);
	free_tree(info->tree);
	free_token_list(info->token_list);
	free_token_list(info->heredocs_list);
	free_args(info->envp);
	rl_clear_history();
	ft_close_fds(info->saved_streams, info);
	remove_terminal_echo(info);
	exit(info->last_exit_status);
	return (OK);
}
