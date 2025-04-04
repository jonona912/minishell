/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 14:00:21 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/03 20:03:59 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h> // for open

#include "../../libft/libft.h"

// # include <sys/types.h>

typedef enum	s_token_type
{
	// no token
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_RED_IN,
	TOKEN_RED_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
	TOKEN_SPACE
}	t_token_type;

// make separate lists for tokens and commands
typedef struct s_cmd_lst
{
	char			*path;  
	char			**av;
	struct s_cmd_lst	*next;
}	t_cmd_lst;

typedef struct s_token_lst
{
	t_token_type	type;
	char			*value;
	struct s_token_lst	*next;
}	t_token_lst;

typedef struct	s_command
{
	char **tokens;
	char *current_cmd_lst;
	char *input_file;
	char *output_file;
	struct s_command *next;
	int	quote;
	int	redirection;
	int	pipe;
}	t_command;


typedef struct s_tokenize_struct
{
	char	*current_token;
	int		is_s_quote;
	int		is_d_quote;
	int		is_redirection;
	int		is_pipe;
}	t_tokenize_struct;











///////////// lists /////////////
// ms_command_lst.c
t_cmd_lst	*ms_new_node(char *value, char **argv);
void		ms_free_list(t_cmd_lst *head);
t_cmd_lst	*ms_get_last_node(t_cmd_lst *head);
void		ms_add_node_back(t_cmd_lst **head, t_cmd_lst *new_node);

// ms_token_lst.c
t_token_lst	*token_new_node(t_token_type type, char *value);
void		token_free_list(t_token_lst *head);
t_token_lst	*token_get_last_node(t_token_lst *head);
void		token_add_node_back(t_token_lst **head, t_token_lst *new_node);

////////////// tokenizer ////////////
// ms_tokenizer.c
int			ft_append_char(char *str, char c);
void		initialize_tokenize_struct(t_tokenize_struct *vars, char *line);
t_token_lst	*ft_tokenize(char *line);

#endif


