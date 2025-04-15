#include "../../includes/minishell.h"

void	ft_echo(char **argv)
{
	int	i;
	int	n;

	n = 0;
	i = 1;
	if (!argv[i])
		return ;
	while (ft_strcmp(argv[i], "-n") == 0)
	{
		if (!argv[i + 1])
			return ;
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

void	ft_env()
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		write(1, environ[i], ft_strlen(environ[i]));
		write(1, "\n", 1);
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
			ft_putstr_fd("Error: OLDPWD is not set\n", 2);
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
	char	*name;
	int		name_len;

	if (!argv[1])
	{
		ft_env();
		return (0);
	}
	equal = ft_strchr(argv[1], '=');
	if (!equal || equal == argv[1])
	{
		perror("Error: invalid syntax input");
		return (1);
	}
	name_len = equal - argv[1];
	name = (char *) malloc(name_len + 1);
	if (!name)
	{
		perror("Error: memory allocation failed");
		return (1);
	}
	ft_strlcpy(name, argv[1], name_len + 1);
	if (ft_setenv(name, equal + 1, 1))
	{
		perror("Error: invalid syntax input");
		free(name);
		return (1);
	}
	free(name);
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
