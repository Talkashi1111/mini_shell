/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/03/25 16:40:53 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

void    free_args(char **args)
{
    int i;

    if (!args)
        return ;
    i = 0;
    while (args[i])
    {
        free(args[i]);
        args[i] = NULL;
        i++;
    }
    free(args);
}

int count_args(char **args)
{
    int i;

    if (!args)
        return (0);
    i = 0;
    while (args[i])
        i++;
    return (i);
}

int	main(int argc, char **argv, char **envp)
{
	char *line;
    char **args;
    int n_args;
    int last_exit_status;

    (void)argc;
    (void)argv;
    (void)envp; //TODO: remove this line after using envp
    last_exit_status = 0;
    while (TRUE) {
        line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
        if (!line)
            break;
        printf(COLOR_RESET "line given: %s\n", line); // TODO: Remove this line
        
        args = ft_split(line, ' ');
        if (!args)
        {
            ft_fprintf(STDERR_FILENO, "split error\n");
            free(line);
            continue;
        }
        if (!args[0])
        {
            free_args(args);
            free(line);
            continue;
        }
        n_args = count_args(args);
        if (n_args == 1 && ft_strncmp(args[0], "exit", sizeof("exit")) == 0) 
        {
            free_args(args);
            free(line);
            break;
        }
        else if (n_args == 2 && ft_strncmp(args[0], "cd", sizeof("cd") == 0))
            last_exit_status = cd_builtin(args[1]);
        free_args(args);
        free(line);
    }
    printf("%d\n", last_exit_status);
    printf(COLOR_GREEN "minishell ~ " COLOR_RED "EXITED\n" COLOR_RESET);
    return (OK);
}
