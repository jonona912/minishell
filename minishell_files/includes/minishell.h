/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 14:00:21 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/10 16:47:04 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <termios.h>
# include <fcntl.h> // for open

#include "../../libft/libft.h"

// # include <sys/types.h>

typedef enum	s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECTION_IN,
	TOKEN_REDIRECTION_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_S_QUOTE,
	TOKEN_D_QUOTE,
	TOKEN_WILDCARD, // not handled
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_L_PAREN,
	TOKEN_R_PAREN,
}	t_token_type;

typedef struct s_token_lst
{
	t_token_type	type;
	char			*value;
	struct s_token_lst	*next;
}	t_token_lst;

// make separate lists for tokens and commands
typedef struct s_cmd_lst
{
	char			*path;
	char			**av;
	struct s_cmd_lst	*next;
}	t_cmd_lst;

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
	int		is_parenthesis;
	int		paren_counter;
}	t_tokenize_struct;


// parser grammar
// precedence rules: redirection_ins or heredoc
// redirection_outs or append

// delimitor is pipe |
// associativity: left: execution commands, right: next_node

// Backus-Naur Form (BNF) grammar


// EXPRESSION ::= TERM (e.g. a - b | a + b)
// TERM ::= FACTOR (e.g. a * b | a / b)
// FACTOR ::= (e.g. a | b | () )

// AST node types
typedef enum {
    NODE_CMD_PIPE,
	NODE_CMD,
	NODE_REDIRECTION_IN,
	NODE_REDIRECTION_OUT,
	NODE_APPEND,
	NODE_HEREDOC
} t_node_type;

// AST node structure
typedef struct s_ast_node {
    t_node_type type; // CMD_PIPE_IN / OUT  | Redirection
    union {
		struct {
        	char *executable;
			char **exec_argv;
		} cmd;
        struct {
            t_token_type op;
            struct s_ast_node* left_exec;
            struct s_ast_node* right_cmd;
        } binary_op;
    } data;
} t_ast_node;




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


int		ft_isblank(int c);
int		handle_unmatched_quotes(t_tokenize_struct *vars, t_token_lst **token_lst);
int		process_redirection(t_tokenize_struct *vars, t_token_lst **token_lst, char *line, int *i, t_token_type token_type, int step);

#endif
