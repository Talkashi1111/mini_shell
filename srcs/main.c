/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/04/26 15:45:32 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens_tree_heredocs(t_minishell *info)
{
	free_tree(info->tree);
	info->tree = NULL;
	free_heredocs_list(info->heredocs_list, info);
	info->heredocs_list = NULL;
	free_token_list(info->token_list);
	info->token_list = NULL;
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
		line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
		no_line(info, line);
		info->token_list = tokenizer(line, info);
		if (DEBUG == 1)
			display_token_list(info->token_list);
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
		getchar();
	}
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell			info;
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	sa.sa_handler = &signal_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
	sigaction(SIGQUIT, &sa, NULL);//??
	sigaction(SIGTSTP, &sa, NULL);
	if (minishell_init(&info, envp) != OK)
		return (info.last_exit_status);
	minishell_loop(&info, NULL);
	return (OK);
}
