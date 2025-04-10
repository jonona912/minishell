# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: opopov <opopov@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/29 13:59:34 by zkhojazo          #+#    #+#              #
#    Updated: 2025/04/09 18:47:47 by opopov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME = minishell
# CC = cc
# CFLAGS = -Wall -Wextra -Werror -g
# INCLUDE = minishell_files/includes/minishell.h

# SRCS_DIR = minishell_files/srcs
# # LISTS_DIR = $(SRCS_DIR)/lists
# PARSER_DIR = $(SRCS_DIR)/parser
# SRCS = main_minishell.c \
# 		$(PARSER_DIR)/tokenizer_helper_1.c \
# 		$(PARSER_DIR)/token_lst_functions.c \
# 		$(PARSER_DIR)/tokenizer.c

# # simplify libft
# LIBFT_DIR = libft
# LIBFT = libft.a

# OBJS_DIR = minishell_files/objs
# OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

# all: $(NAME)
# #./minishell
# $(NAME): $(OBJS) $(INCLUDE) $(LIBFT_DIR)/$(LIBFT)
# 	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_DIR)/$(LIBFT) -lreadline

# $(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
# 	mkdir -p $(OBJS_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJS_DIR)/%.o:
# 	mkdir -p $(OBJS_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJS_DIR)/%.o: $(PARSER_DIR)/%.c
# 	mkdir -p $(OBJS_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(LIBFT_DIR)/$(LIBFT):
# 	$(MAKE) -C $(LIBFT_DIR)

# clean:
# 	rm -rf $(OBJS_DIR)
# 	$(MAKE) -C $(LIBFT_DIR) clean

# fclean: clean
# 	rm -f $(NAME)
# 	$(MAKE) -C $(LIBFT_DIR) fclean

# re: fclean all


NAME = minishell
TESTER = token_tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDE = minishell_files/includes/minishell.h

SRCS_DIR = minishell_files/srcs
PARSER_DIR = $(SRCS_DIR)/parser
SRCS = main_minishell.c \
		$(PARSER_DIR)/tokenizer_helper_1.c \
		$(PARSER_DIR)/token_lst_functions.c \
		$(PARSER_DIR)/tokenizer.c

# Test files
TEST_DIR = tests
TEST_SRC = $(TEST_DIR)/token_tester.c

OBJS_DIR = minishell_files/objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
TEST_OBJ = $(addprefix $(OBJS_DIR)/, $(TEST_SRC:.c=.o))

# simplify libft
LIBFT_DIR = libft
LIBFT = libft.a

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDE) $(LIBFT_DIR)/$(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_DIR)/$(LIBFT) -lreadline

$(TESTER): $(TEST_OBJ) $(filter-out $(OBJS_DIR)/main_minishell.o, $(OBJS)) $(LIBFT_DIR)/$(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT_DIR)/$(LIBFT) -lreadline

# Rule for source files in SRCS_DIR
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for source files in PARSER_DIR
$(OBJS_DIR)/$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for test files
$(OBJS_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_DIR)/$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

test: $(TESTER)
	./$(TESTER)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(TESTER)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re test
