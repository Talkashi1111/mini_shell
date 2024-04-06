/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 20:42:00 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

static int init_info(t_minishell *info, char **envp)
{
    info->envp = copy_env(envp);
    if (!info->envp || update_or_add_envp(info, "OLDPWD=", "") == MALLOC_ERROR)
        return (MALLOC_ERROR);
    info->last_exit_status = 0;
    info->buildins[0] = (t_builtin){"cd", ft_cd};
    info->buildins[1] = (t_builtin){"echo", ft_echo};
    info->buildins[2] = (t_builtin){"env", ft_env};
    info->buildins[3] = (t_builtin){"exit", ft_exit};
    info->buildins[4] = (t_builtin){"export", ft_export};
    info->buildins[5] = (t_builtin){"pwd", ft_pwd};
    info->buildins[6] = (t_builtin){"unset", ft_unset};
    info->buildins[7] = (t_builtin){NULL, NULL};
    return (OK);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	    info;
    t_pfunc         func;
    char            **args;
    char            *line;
   // t_token_list    *test_wildcard;//TODO: remove this line

    (void)argc;
    (void)argv;
    if (init_info(&info, envp) == MALLOC_ERROR)
        return (MALLOC_ERROR);
    //test_wildcard = get_wildcard("*t*"); //TODO: remove this line
    //display_token_list(test_wildcard); //TODO: remove this line
   // free_token_list(test_wildcard); //TODO: remove this line
    while (TRUE)
    {
        line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
        if (!line)
        {
            ft_exit(NULL, &info);
            break;
        }
        info.token_list = tokenizer(line);
        if (DEBUG == TRUE)
            display_token_list(info.token_list);
        if (info.token_list == NULL)
        {
            free(line);
            continue ;
        }
        add_history(line);
        free(line);
        func = is_builtin(info.token_list->str, info.buildins);
        if (func)
        {
            args = tokens_to_args(info.token_list);
            if (args != NULL)
                info.last_exit_status = func(args, &info);
            free_args(args);
        }
        else
            ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", info.token_list->str);
		if (expand_dollars(info.token_list, &info) != OK)
			ft_printf("ERROR\n");
		if (remove_quotes(info.token_list) != OK)
            ft_printf("ERROR\n");
        if (syntax_analyser(info.token_list) != OK)
        {
            free_token_list(info.token_list);
            continue ;
        }
        info.tree = tree_maker(info.token_list, NULL);
        if (info.tree == NULL)
        {
            free_token_list(info.token_list);
            continue ;
        }
        if (DEBUG == TRUE)
            display_tree(info.tree);
        free_tree(info.tree);
        free_token_list(info.token_list);
        ft_printf("exit status=%d\n", info.last_exit_status);
    }
    return (OK);
}
