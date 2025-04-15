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
#include <limits.h>
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

typedef struct s_tokenize_struct
{
	char	*current_token;
	int		is_s_quote;
	int		is_d_quote;
	int		is_parenthesis;
	int		paren_counter;
}	t_tokenize_struct;



// // make separate lists for tokens and commands
// typedef struct s_cmd_lst
// {
// 	char			*path;
// 	char			**av;
// 	struct s_cmd_lst	*next;
// }	t_cmd_lst;

// typedef struct	s_command
// {
// 	char **tokens;
// 	char *current_cmd_lst;
// 	char *input_file;
// 	char *output_file;
// 	struct s_command *next;
// 	int	quote;
// 	int	redirection;
// 	int	pipe;
// }	t_command;



typedef enum {
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
} t_node_type;




// // AST node structure
// typedef struct s_ast_node
// {
// 	t_node_type type; // CMD or PIPE
// 	union {
// 		struct
// 		{
// 			char *executable;
// 			char **exec_argv;
// 			t_redirection *redirs;
// 		} cmd;
// 		struct
// 		{
// 			t_token_type op; // not needed probably
// 			int		pipe_fd[2]; // also not used
// 			struct s_ast_node* left;
// 			struct s_ast_node* right;
// 		} binary_op;
// 	} data;
// } t_ast_node;

// Structure for a single redirection or heredoc
typedef struct s_redir_lst {
    t_token_type type;      // REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC
    char *target;           // File path (e.g., "input.txt") or heredoc delimiter (e.g., "EOF")
    struct s_redir_lst *next; // Linked list for multiple redirections
} t_redir_lst;

// AST node structure
typedef struct s_ast_node
{
	t_node_type type; // CMD or PIPE
	union {
		struct
		{
			char *executable;
			char **exec_argv;
			t_redir_lst *redirs;
		} cmd;
		struct
		{
			struct s_ast_node* left;
			struct s_ast_node* right;
		} binary_op;
	} data;
} t_ast_node;

// typedef struct s_pid_lst
// {
// 	pid_t				pid;
// 	struct s_pid_lst	*next;
// }	t_pid_lst;






///////////// lists /////////////
// ms_command_lst.c
// t_cmd_lst	*ms_new_node(char *value, char **argv);
// void		ms_free_list(t_cmd_lst *head);
// t_cmd_lst	*ms_get_last_node(t_cmd_lst *head);
// void		ms_add_node_back(t_cmd_lst **head, t_cmd_lst *new_node);

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
int		ft_setenv(char *name, char *value, int overwrite);
void	ft_echo(char **argv);
void	ft_pwd();
int		ft_exit(char **argv);
void	ft_env();
int		ft_cd (char **argv);
int		ft_export(char **argv);
int		ft_unset(char **argv);

/////////////////////// parser_2 /////////////////////////

// ast_binary_tree_function.c
t_ast_node *create_cmd_node(t_node_type type, char *executable, char **exec_argv, t_redir_lst *redirs);
t_ast_node *create_binary_op_node(t_node_type type, t_ast_node *left, t_ast_node *right);

// parser.c
int	is_redirection(t_token_type type);
t_token_lst	*parse_pipe(t_token_lst *token_lst, t_ast_node **ast_node);

// redirection_functions.c
t_redir_lst *new_redir_node(t_token_type type, char *target);
void add_redir_back(t_redir_lst **lst, t_redir_lst *new_node);
void free_redir_list(t_redir_lst **lst);






///////////////////// execute ////////////////////////
// int execute(t_ast_node *ast_head, int pipe_direction, int pipe_fd[2]);
void run_pipeline(t_ast_node *ast_head);


#endif
