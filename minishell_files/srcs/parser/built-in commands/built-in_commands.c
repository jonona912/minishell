/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:35:48 by opopov            #+#    #+#             */
/*   Updated: 2025/04/14 17:45:00 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_echo(char **argv)
{
	int	i;
	int	n;

	n = 0;
	i = 1;
	while (ft_strcmp(argv[i], "-n") == 0)
	{
		n = 1;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n)
		write(1, "\n", 1);
}

void	ft_pwd()
{
	char cwd[4096];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_putstr_fd("Error: path is not found\n", 2);
}

int	ft_exit(char **argv)
{
	if (argv[1])
	{
		write(1, "Error: too many arguments\n", 26);
		return (1);
	}
	exit(0);
}

void	ft_env(char **argv)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		write(1, environ[i], ft_strlen(environ[i]));
		i++;
	}
}

int	ft_cd (char **argv)
{
	char		cwd[4096];
	static char	*oldpwd = NULL;
	char		new_cwd[4096];
	char		*tmp;

	tmp = argv[1];
	// case for cd and cd ~
	if (!argv[1] || strcmp(argv[1], "~") == 0)
	{
		tmp = getenv("HOME");
		if (!tmp)
		{
			ft_putstr_fd("Error: HOME is not set\n", 2);
			return (1);
		}
	}
	// case for cd -
	else if (strcmp(argv[1], "-") == 0)
	{
		if (oldpwd)
			tmp = oldpwd;
		else
			tmp = getenv("OLDPWD");
		if (!tmp)
		{
			ft_putchar_fd("Error: OLDPWD is not set\n", 2);
			return (1);
		}
	}
	// save current directory
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("Error: current working directory name not found\n", 2);
		return (1);
	}
	// change directory
	if (chdir(tmp) != 0)
	{
		ft_putstr_fd("Error: directory cannot be changed\n", 2);
		return (1);
	}
	// set OLDPWD to previous directory
	if (ft_setenv("OLDPWD", cwd, 1))
		return (1);
	// set PWD to new directory
	if (!getcwd(new_cwd, sizeof(new_cwd)))
	{
		ft_putstr_fd("Error: cannot set PWD to new directory\n", 2);
		return (1);
	}
	// update oldpwd
	if (oldpwd)
		free(oldpwd);
	oldpwd = ft_strdup(cwd);
	return (0);
}

int	ft_export(char **argv)
{
	extern char	**environ;
	char	*equal;
	int		i;
	int		env_y;
	char	**env_2;
	char	*tmp;

	if (!argv[1])
	{
		ft_env(argv);
		return (0);
	}
	equal = ft_strchr(argv[1], '=');
	if (!equal)
	{
		ft_putstr_fd("Error: invalit syntax input\n", 2);
		return (1);
	}
	if (ft_setenv(argv[1], equal + 1, 1))
	{
		ft_putstr_fd("Error: invalit syntax input\n", 2);
		return (1);
	}
	return (0);
}

int	ft_unset(char **argv)
{
	int			i;
	extern char	**environ;
	char		*equal;
	int			j;

	i = 0;
	if (!argv[1])
	{
		ft_putstr_fd("Error: missing argument for unset", 2);
		return (1);
	}
	while (environ[i])
	{
		equal = ft_strchr(environ[i], '=');
		if (equal && ft_strncmp(environ[i], argv[1], equal - environ[i]) == 0)
		{
			free(environ[i]);
			j = i;
			while (environ[j])
			{
				environ[j] = environ[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}
