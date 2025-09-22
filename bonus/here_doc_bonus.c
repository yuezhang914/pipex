/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 00:23:28 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/22 12:36:27 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	write_all(int fd, const char *buf, size_t len)
{
	size_t	written;
	ssize_t	w;

	if (!buf)
		return (0);
	written = 0;
	while (written < len)
	{
		w = write(fd, buf + written, len - written);
		if (w < 0)
		{
			if (errno == EINTR)
				continue ;
			return (-1);
		}
		written += (size_t)w;
	}
	return (0);
}

static int	process_line(int hd_w, char **line, const char *limiter)
{
	size_t	len;

	len = ft_strlen(*line);
	if (len > 0 && (*line)[len - 1] == '\n')
	{
		(*line)[len - 1] = '\0';
		len = len - 1;
	}
	if (!ft_strncmp(*line, limiter, ft_strlen(limiter) + 1))
	{
		free(*line);
		*line = NULL;
		return (1);
	}
	if (write_all(hd_w, *line, len) < 0 || write_all(hd_w, "\n", 1) < 0)
	{
		free(*line);
		*line = NULL;
		return (-1);
	}
	free(*line);
	*line = NULL;
	return (0);
}

static int	step_here_doc(int *hd, char **line, const char *limiter)
{
	int	ret;

	(void)write(2, "heredoc> ", 9);
	*line = get_next_line(0);
	if (*line == NULL)
		return (1);
	ret = process_line(hd[1], line, limiter);
	if (ret < 0)
		return (-2);
	if (ret == 1)
		return (1);
	return (0);
}

static int	fail_and_close(int *hd, char **line)
{
	if (line && *line)
	{
		free(*line);
		*line = NULL;
	}
	close(hd[1]);
	close(hd[0]);
	return (-1);
}

int	do_here_doc(char *limiter)
{
	int		hd[2];
	char	*line;
	int		s;

	line = NULL;
	if (pipe(hd) < 0)
		return (-1);
	while (1)
	{
		s = step_here_doc(hd, &line, limiter);
		if (s == -2)
			return (fail_and_close(hd, &line));
		if (s == 1)
			break ;
	}
	if (line)
		free(line);
	close(hd[1]);
	return (hd[0]);
}
