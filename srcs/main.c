/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/04/19 19:50:57 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static int minishell_init(t_minishell *info, char **envp)
{
    info->envp = copy_env(envp);
    if (!info->envp || update_or_add_envp(info, "OLDPWD=", "") != OK)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
        return (info->last_exit_status);
	}
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
	save_std_streams(info);
	return (OK);
}

void    free_tokens_tree_heredocs(t_minishell *info)
{
    free_tree(info->tree);
    info->tree = NULL;
    free_token_list_and_heredocs(info->token_list, info);
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
			info->last_exit_status = OK;
			ft_printf("exit\n");
			ft_exit(NULL, info);
		}
        info->token_list = tokenizer(line, info);
        if (info->token_list)
        {
            add_history(line);
            free(line);
            line = NULL;
            if (syntax_analyser(info->token_list, info) == OK)
            {
                info->tree = tree_maker(info->token_list, NULL, info);
				display_tree(info->tree);
				display_token_list(info->token_list);
             /*    if (!info->tree)//to check if there was an error no?
                {
                    info->last_exit_status = errno;
                    free_tokens_tree_heredocs(info);
                    //break??
                } */
                if (info->tree)
                    ft_run(info->tree, info);
            }
        }
        free(line);
        free_tokens_tree_heredocs(info);
    }
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);// sert a rien sur linux
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell 		info;
	struct sigaction	sa;

    (void)argc;
    (void)argv;
	sa.sa_handler = &signal_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
    if (minishell_init(&info, envp) != OK)
        return (info.last_exit_status);
    minishell_loop(&info);
    return (OK);
}
