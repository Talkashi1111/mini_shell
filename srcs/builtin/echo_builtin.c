/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:44:43 by achappui          #+#    #+#             */
/*   Updated: 2024/04/18 14:07:24 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char *args[], t_minishell *info)
{
	int	i;
	int	option_n;
	int	n_args;

	(void)info->envp;
	n_args = count_args(args);
	option_n = FALSE;
	i = 1;
	if (n_args > 1 && ft_strncmp(args[1], "-n", sizeof("-n")) == 0)
	{
		option_n = TRUE;
		i++;
	}
	while (i < n_args)
	{
		ft_printf("%s", args[i]);
		if (i < n_args - 1)
			ft_printf(" ");
		i++;
	}
	if (!option_n)
		ft_printf("\n");
	return (OK);
}
