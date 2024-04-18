/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:19:55 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/17 22:49:35 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char *args[], t_minishell *info)
{
	free_args(args);
	free_tree(info->tree, info);
	free_token_list(info->token_list, 0, info);
	free_args(info->envp);
	exit(info->last_exit_status);
	return (OK);
}
