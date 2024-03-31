/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/03/31 19:18:38 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

void ft_exit(char *args[], char **envp[])
{
    free_args(args);
    free_args(*envp);
    *envp = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	char    *line;
    char    **args;
    char    **envp_cpy;
    int last_exit_status;

    (void)argc;
    (void)argv;
    envp_cpy = copy_env(envp);
    if (!envp_cpy  || update_or_add_envp(&envp_cpy, "OLDPWD=", "") == MALLOC_ERROR)
        return (MALLOC_ERROR);
    last_exit_status = 0;
    while (TRUE)
    {
        line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
        if (!line)
        {
            ft_exit(NULL, &envp_cpy);
            break;
        }
        args = ft_split(line, ' ');
        free(line);
        if (!args)
        {
            ft_fprintf(STDERR_FILENO, "split error\n");
            continue;
        }
        if (!args[0])
        {
            free_args(args);
            continue;
        }
        add_history(line);
        if (ft_strncmp(args[0], "exit", sizeof("exit")) == 0) 
        {
            ft_exit(args, &envp_cpy);
            break;
        }
        else if (ft_strncmp(args[0], "cd", sizeof("cd")) == 0)
            last_exit_status = ft_cd(args, &envp_cpy);
        else if (ft_strncmp(args[0], "echo", sizeof("echo")) == 0)
            last_exit_status = ft_echo(args, &envp_cpy);
        else if (ft_strncmp(args[0], "env", sizeof("env")) == 0)
            last_exit_status = ft_env(args, &envp_cpy);
        else if (ft_strncmp(args[0], "pwd", sizeof("pwd")) == 0)
            last_exit_status = ft_pwd(args, &envp_cpy);
        else if (ft_strncmp(args[0], "export", sizeof("export")) == 0)
            last_exit_status = ft_export(args, &envp_cpy);
        else if (ft_strncmp(args[0], "unset", sizeof("unset")) == 0)
            last_exit_status = ft_unset(args, &envp_cpy);
        else
            ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
        free_args(args);
        ft_printf("%d\n", last_exit_status);
    }
    ft_printf(COLOR_GREEN "minishell ~ " COLOR_RED "EXITED\n" COLOR_RESET);
    return (OK);
}
