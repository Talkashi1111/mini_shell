/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/04/07 22:20:22 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

static int minishell_init(t_minishell *info, char **envp)
{
    info->envp = copy_env(envp);
    if (!info->envp || update_or_add_envp(info, "OLDPWD=", "") == MALLOC_ERROR)
        return (MALLOC_ERROR);
    info->last_exit_status = OK;
    info->token_list = NULL;
    info->tree = NULL;
    info->builtins[0] = (t_builtin){"cd", ft_cd};
    info->builtins[1] = (t_builtin){"echo", ft_echo};
    info->builtins[2] = (t_builtin){"env", ft_env};
    info->builtins[3] = (t_builtin){"exit", ft_exit};
    info->builtins[4] = (t_builtin){"export", ft_export};
    info->builtins[5] = (t_builtin){"pwd", ft_pwd};
    info->builtins[6] = (t_builtin){"unset", ft_unset};
    info->builtins[7] = (t_builtin){NULL, NULL};
    info->fd_pipe = NULL;
    info->pipe_nb = 0;

    return (OK);
}

void    free_iteration_memory(t_minishell *info)
{
    free_tree(info->tree);
    info->tree = NULL;
    free_token_list(info->token_list);
    info->token_list = NULL;
}

char    minishell_loop(t_minishell *info)
{
    char    *line;

    while (TRUE)
    {
        line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
        if (!line)
        {
            ft_fprintf(STDERR_FILENO, "readline error\n");
            info->last_exit_status = 1;
            ft_exit(NULL, info);
        }
        info->token_list = tokenizer(line);
        if (DEBUG == TRUE)
            display_token_list(info->token_list);
        if (info->token_list)
        {
            add_history(line);
            free(line);
            line = NULL;
            if (syntax_analyser(info->token_list) == OK)
            {
                info->tree = tree_maker(info->token_list, NULL);
                if (DEBUG == TRUE)
                    display_tree(info->tree);
                if (info->tree)
                    ft_run(info->tree, info);
            }
        }
        free(line);
        free_iteration_memory(info);
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell info;

    (void)argc;
    (void)argv;
    if (minishell_init(&info, envp) == MALLOC_ERROR)
        return (MALLOC_ERROR);
    minishell_loop(&info);
    return (OK);
}