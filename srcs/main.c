/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/05/26 14:24:56 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_shell_state	g_signal;

void	free_tokens_tree_heredocs(t_minishell *info)
{
	free_tree(info->tree);
	info->tree = NULL;
	free_heredocs_list(info->heredocs_list, info);
	info->heredocs_list = NULL;
	free_token_list(info->token_list);
	info->token_list = NULL;
}

void	signal_printer(void)
{
	if (g_signal == INEXECUTION_SIGINT)
		ft_fprintf(2, "^C\n");
	else if (g_signal == INEXECUTION_SIGQUIT)
		ft_fprintf(2, "^\\Quit: 3\n");
}

/**
 * The main function that runs the minishell loop.
 * to display tree run make with DEBUG=1 and add the following after tree
 * display_tree(info->tree);
 * @param info The t_minishell structure containing
 * information about the minishell.
 * @return The exit status of the minishell.
 */
void	minishell_loop(t_minishell *info, char *line)
{
	while (TRUE)
	{
		signal_printer();
		g_signal = INREADLINE;
		line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
		g_signal = INEXECUTION;
		no_line(info, line);
		info->token_list = tokenizer(line, info);
		if (info->token_list)
		{
			add_history(line);
			free(line);
			line = NULL;
			if (syntax_analyser(info->token_list, info) == OK)
			{
				info->tree = tree_maker(info->token_list, NULL, info);
				if (DEBUG == 1)
					display_tree(info->tree);
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
	if (sig == SIGINT && g_signal == INREADLINE)
	{
		rl_replace_line("", 0);
		ft_fprintf(1, "\n");
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT && g_signal == INREADLINE)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGINT && g_signal == INEXECUTION)
		g_signal = INEXECUTION_SIGINT;
	else if (sig == SIGQUIT && g_signal == INEXECUTION)
		g_signal = INEXECUTION_SIGQUIT;
	else if (sig == SIGHUP)
		return ;
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell			info;
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	sa.sa_handler = &signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	if (minishell_init(&info, envp) != OK)
		return (info.last_exit_status);
	minishell_loop(&info, NULL);
	return (OK);
}
