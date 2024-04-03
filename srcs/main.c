/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/04/02 23:47:05 by achappui         ###   ########.fr       */
/*   Updated: 2024/04/02 21:57:57 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit(char *args[], char **envp[])
{
    free_args(args);
    free_args(*envp);
    *envp = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	  t_minishell	info;
    char    **args;
    char    **envp_cpy;
    int last_exit_status;
    t_token_list	*token_list;
    t_token_list    *test_wildcard;//TODO: remove this line
    t_node  *root;

    (void)argc;
    (void)argv;
    info.envp = copy_env(envp);
    if (!info.envp  || update_or_add_envp(&info.envp, "OLDPWD=", "") == MALLOC_ERROR)
        return (MALLOC_ERROR);
    info.last_exit_status = 0;
    last_exit_status = 0;
    test_wildcard = get_wildcard("*t*"); //TODO: remove this line
    display_token_list(test_wildcard); //TODO: remove this line
    free_token_list(test_wildcard); //TODO: remove this line
    while (TRUE)
    {
        info.line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
        if (!info.line)
        {
            ft_exit(NULL, &info.envp);
            break;
        }

        info.token_list = tokenizer(info.line);
        if (info.token_list == NULL)
            continue ;
		if (expand_dollars(info.token_list, &info) == 2)
			printf("ERROR\n");
		// if (remove_quotes(info.token_list) == 2)
		// 	printf("ERROR\n");
        display_token_list(info.token_list); // TODO: remove this info.line
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
        //display_tree(info.tree); // TODO: remove this info.line
        free_tree(info.tree);
        free_token_list(info.token_list);


        args = ft_split(info.line, ' ');
        free(info.line);
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
        add_history(info.line);
        if (ft_strncmp(args[0], "exit", sizeof("exit")) == 0) 
        {
            ft_exit(args, &info.envp);
            break;
        }
        else if (ft_strncmp(args[0], "cd", sizeof("cd")) == 0)
            info.last_exit_status = ft_cd(args, &info.envp);
        else if (ft_strncmp(args[0], "echo", sizeof("echo")) == 0)
            info.last_exit_status = ft_echo(args, &info.envp);
        else if (ft_strncmp(args[0], "env", sizeof("env")) == 0)
            info.last_exit_status = ft_env(args, &info.envp);
        else if (ft_strncmp(args[0], "pwd", sizeof("pwd")) == 0)
            info.last_exit_status = ft_pwd(args, &info.envp);
        else if (ft_strncmp(args[0], "export", sizeof("export")) == 0)
            info.last_exit_status = ft_export(args, &info.envp);
        else if (ft_strncmp(args[0], "unset", sizeof("unset")) == 0)
            info.last_exit_status = ft_unset(args, &info.envp);
        else
            ft_fprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
        free_args(args);
        //ft_printf("%d\n", info.last_exit_status);
    }
    ft_printf(COLOR_GREEN "minishell ~ " COLOR_RED "EXITED\n" COLOR_RESET);
    return (OK);
}
