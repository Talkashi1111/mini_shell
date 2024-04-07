/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:19:55 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/07 16:36:24 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

// TODO: check that we didn't forget to free something
int	ft_exit(char *args[], t_minishell *info)
{
    free_args(args);
	free_tree(info->tree);
	free_token_list(info->token_list);
    free_args(info->envp);
	exit(info->last_exit_status);
	return (OK);
}