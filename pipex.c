/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansel <achansel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 19:13:52 by achansel          #+#    #+#             */
/*   Updated: 2022/03/27 22:51:43 by achansel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#define SUCCESS 0
#define ERROR 1

typedef struct s_exec
{
	char	**av;
	char	*path;
	char	**paths;
	char	bp[2048];
	int		pid;
	int		i;
}	t_exec;

char	*strcat_mod(char *dst, char *str1, char *str2, int str1_len)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (str1[++i] && i < str1_len)
		dst[i] = str1[i];
	if (str2)
		dst[i++] = '/';
	while (str2 && str2[++j])
		dst[i + j] = str2[j];
	if (!str2)
		dst[i + 1 + j] = 0;
	else
		dst[i + j] = 0;
	return (dst);
}

char	**split(char *cmd, char delim)
{
	char	**hihi;
	char	*s;
	int		i;

	hihi = malloc(1000 * sizeof(char *));
	i = 0;
	while (i < 1000)
		hihi[i++] = NULL;
	i = 0;
	if (cmd <= (char *) 1024)
		exit(write(1, "Error: PATH is unset\n", 21));
	s = NULL;
	while (*(++cmd))
	{
		if (*cmd == delim && s != NULL)
		{
			hihi[i++] = strcat_mod(malloc((cmd - s) + 1), s, 0, cmd - s);
			s = NULL;
		}
		if (*cmd != delim && s == NULL)
			s = cmd;
	}
	if (s != NULL)
		hihi[i++] = strcat_mod(malloc((cmd - s) + 1), s, 0, cmd - s);
	return (hihi);
}

void	make_exec(char *cmd, int redirs[2], char **envp)
{
	t_exec	e;
	int		i;

	i = 0;
	e.pid = fork();
	if (e.pid == -1)
		write(2, "Fork error\n", 11);
	else if (e.pid == 0)
	{
		while (envp[i] && !(envp[i][0] == 'P' && envp[i][1] == 'A' && \
			envp[i][2] == 'T' && envp[i][3] == 'H' && envp[i][4] == '='))
			i++;
		e.path = envp[i] + 5;
		e.paths = split(e.path - 1, ':') - 1;
		e.av = split(cmd - 1, ' ');
		while (*(++(e.paths)))
			if (access(strcat_mod(e.bp, *(e.paths), e.av[0], 2048), X_OK) == 0)
				break ;
		if (access(e.bp, X_OK))
			exit(write(2, "no such command\n", 16));
		dup2(redirs[1], 1);
		dup2(redirs[0], 0);
		if (execve(e.bp, e.av, envp) == -1)
			exit(write(2, "execve error\n", 13));
	}
}

int	pipex(int *fds, char **cmd, char **envp)
{
	int		redirs[2];
	int		pipe_fds[2];
	int		last_fd;

	last_fd = -1;
	while (*cmd)
	{
		redirs[0] = last_fd;
		if (*(cmd + 1) != NULL)
		{
			pipe(pipe_fds);
			redirs[1] = pipe_fds[1];
			if (last_fd == -1)
				redirs[0] = fds[0];
			last_fd = pipe_fds[0];
		}
		else
			redirs[1] = fds[1];
		make_exec(*cmd, redirs, envp);
		if (*(cmd + 1) != NULL)
			close(pipe_fds[1]);
		cmd++;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	char	**cmd;
	int		fds[2];

	if (argc != 5)
	{
		write(2, "Error:\n", 7);
		write(2, "usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
		return (ERROR);
	}
	fds[0] = open(argv[1], O_RDONLY | O_CLOEXEC);
	if (fds[0] == -1)
		return (write(2, "Couldn't open file\n", 19));
	fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR
			| S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fds[1] == -1)
		return (write(2, "Coudln't write to file\n", 23));
	cmd = &argv[2];
	argv[argc - 1] = NULL;
	if (cmd[0][0] == '\0' || cmd[1][0] == '\0')
		return (write(2, "Please specify commands\n", 24));
	if (!envp[0])
		return (write(2, "env is unset\n", 13));
	pipex(fds, cmd, envp);
	close(fds[1]);
	return (!wait(NULL));
}
