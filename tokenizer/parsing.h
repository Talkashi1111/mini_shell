/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 14:52:57 by achappui          #+#    #+#             */
/*   Updated: 2024/03/17 14:52:57 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <fcntl.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>

//Tokens types
# define WORD			0
# define STDIN			1
# define STDIN_HEREDOC	2
# define STDOUT			3
# define STDOUT_APPEND	4
# define OR				5
# define AND			6
# define PIPE			7
// # define END			8



typedef struct s_token_list
{
	char				*str;
	char				type;
	struct s_token_list	*next;
}	t_token_list;

// //list of redirections to be applied for a t_cmd node
// typedef struct s_redi
// {
// 	char			type;
// 	char			*arg;
// 	struct s_redi	*next;
// }	t_redi;

// //list of command to be applied in a t_cmd_group
// typedef struct s_cmd
// {
// 	char			*cmd;
// 	char			*args;
// 	struct s_redi	*redi_list;
// }	t_cmd;

// //list of command groups (a pipeline or a single command) delimited by && or ||
// typedef struct s_cmd_group
// {
// 	unsigned int		pipe_nb;
// 	struct s_cmd		*cmds;
// 	char				group_delimiter;
// }	t_cmd_group;

#endif