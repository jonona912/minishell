/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:16:52 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/09 10:03:31 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isblank(int c)
{
	return (c == ' ' || c == '\t');
}

int	ft_append_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	str[i] = c;
	str[i + 1] = '\0';
	return (1);
}

void	initialize_tokenize_struct(t_tokenize_struct *vars, char *line)
{
	int	line_len;

	line_len = ft_strlen(line);
	vars->current_token = (char *)malloc((line_len + 1) * sizeof(char));
	if (!vars->current_token)
	{
		printf("Error: memory allocation failed\n");
		exit(1); ;
	}
	vars->current_token[0] = '\0';
	vars->is_s_quote = 0;
	vars->is_d_quote = 0;
	vars->is_parenthesis = 0;
	vars->type = TOKEN_WORD;
	vars->d_counter = 0;
	vars->s_counter = 0;
}
