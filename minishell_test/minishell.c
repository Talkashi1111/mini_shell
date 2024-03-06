/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/02/23 18:56:10 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	char *line;

    line = readline("\033[0;32mminishell ~ \033[0m");
    while (ft_strncmp(line, "exit", 5) != 0)
    {
        printf("\033[0mline given: %s\n", line);
    	free(line);
     	line = readline("\033[0;32mminishell ~ \033[0m");
    }
    free(line);
	printf("\033[0;32mminishell ~ \033[0;31mEXITED\033[0m\n");
	return (0);
}
