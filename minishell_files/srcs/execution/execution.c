/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:54:11 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/04/06 19:54:20 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_count_argv(t_token_lst *token_lst)
{
	int	count;

	count = 0;
	while (token_lst && ft_strcmp(token_lst->value, "|") != 0)
	{
		count++;
		token_lst = token_lst->next;
	}
	return (count);
}

t_cmd_lst	*ms_create_cmd_lst(t_token_lst *token_lst) // forget about pipes for now
{
	t_cmd_lst	*cmd_lst;
	int			argv_count;
	int			i;
	char		*path;
	char		**temp_argv;
	i = 0;

	cmd_lst = NULL;
	if (!token_lst)
		return (NULL);
	argv_count = ms_count_argv(token_lst);
	temp_argv = (char **)malloc(sizeof(char *) * (argv_count + 1));
	if (!temp_argv)
		return (NULL); // improve
	if (i == 0)
		path = ft_strjoin("/bin/", token_lst->value);
	while (token_lst && ft_strcmp(token_lst->value, "|") != 0)
	{
		temp_argv[i] = ft_strdup(token_lst->value);
		if (!temp_argv[i])
			return (NULL); // improve
		i++;
		token_lst = token_lst->next;
	}
	temp_argv[i] = NULL;
	t_cmd_lst *new_node = ms_new_node(path, temp_argv);
	if (!new_node)
		return (NULL); // improve
	ms_add_node_back(&cmd_lst, new_node);
	return (cmd_lst);
}

int	execute_cmd(t_cmd_lst *cmd_lst)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (!cmd_lst)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		dup2(1, STDOUT_FILENO);  // Redirect stdout to the pipe
		if (execve(cmd_lst->path, cmd_lst->av, NULL) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else
		waitpid(pid, &status, 0);
	return (status);
}
