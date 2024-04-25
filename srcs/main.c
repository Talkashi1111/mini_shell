/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:38:15 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 13:18:50 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	free_tokens_tree_heredocs(t_minishell *info)
{
	free_tree(info->tree);
	info->tree = NULL;
	free_heredocs_list(info->heredocs_list, info);
	info->heredocs_list = NULL;
	free_token_list(info->token_list);
	info->token_list = NULL;
}

char	minishell_loop(t_minishell *info)
{
	char	*line;

	while (TRUE)
	{
		line = readline(COLOR_GREEN "minishell ~ " COLOR_RESET);
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
		rl_replace_line("", 0);
		write(1, "\n", 1);
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
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	if (minishell_init(&info, envp) != OK)
		return (info.last_exit_status);
	minishell_loop(&info);
	return (OK);
}
