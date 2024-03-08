/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/03/08 21:08:29 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "minishell.h"

int	main()
{
	char *line;

    line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
    while (ft_strncmp(line, "exit", sizeof("exit")) != 0)
    {
        printf(COLOR_RESET "line given: %s\n", line);
    	free(line);
     	line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
    }
    free(line);
	printf(COLOR_GREEN "minishell ~ " COLOR_RED "EXITED\n" COLOR_RESET);
	return (OK);
}
