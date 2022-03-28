/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansel <achansel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 23:16:57 by achansel          #+#    #+#             */
/*   Updated: 2022/03/28 12:40:55 by achansel         ###   ########.fr       */
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

char	*strcat_mod(char *dst, char *str1, char *str2, int str1_len);
char	*ft_strncat(char *dest, char *src, int sz);

int	ft_putstr_fd(const char *s, int fd)
{
	while (*s)
		write(fd, s++, 1);
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s2++;
		s1++;
	}
	return ((unsigned char) *s1 - *s2);
}

int	check_params(int argc, char **argv, char **envp)
{
	int		i;
	int		fds[2];
	char	**cmd;

	i = -1;
	if (ft_strcmp(argv[1], "here_doc"))
	{
		fds[0] = open(argv[1], O_RDONLY | O_CLOEXEC);
		if (fds[0] == -1)
			return (write(2, "Couldn't open file\n", 19));
		close(fds[0]);
	}
	fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT, S_IRUSR
			| S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fds[1] == -1)
		return (ft_putstr_fd("Coudln't open file for output\n", 2));
	close(fds[1]);
	cmd = &argv[2];
	while (++i < argc - 3)
		if (cmd[i][0] == '\0')
			return (ft_putstr_fd("Please do not leave empty commands\n", 2));
	if (!envp[0])
		return (write(2, "env is unset\n", 13));
	return (0);
}

int	display_help(void)
{
	ft_putstr_fd("Usage:\n", 2);
	ft_putstr_fd("./pipex file1 cmd1 cmd2 file2              \t\t\t\tOR\n", 2);
	ft_putstr_fd("./pipex here_doc LIMITER cmd1 cmd2 file2   \t\t\t\tOR\n", 2);
	ft_putstr_fd("./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\t\t\t\tOR\n", 2);
	ft_putstr_fd("./pipex here_doc LIMITER cmd1 cmd2 cmd3 ...cmdn file2\n", 2);
	return (1);
}

int	handle_heredocs(char *limiter)
{
	int		fds[2];
	char	input[65536];
	char	line[65536];
	int		i;

	pipe(fds);
	ft_putstr_fd("heredoc> ", 1);
	i = read(0, line, 65536);
	line[i - 1] = '\0';
	while (ft_strcmp(line, limiter))
	{
		ft_strncat(input, line, 65536);
		ft_strncat(input, "\n", 65536);
		ft_putstr_fd("heredoc> ", 1);
		i = read(0, line, 65536);
		line[i - 1] = '\0';
	}
	ft_putstr_fd(input, fds[1]);
	close(fds[1]);
	return (fds[0]);
}
