/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:58:03 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 03:21:56 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	*alloc_pipes(int pipe_nmbs)
{
	int	*arr;
	int	i;

	if (pipe_nmbs <= 0)
		return (NULL);
	arr = malloc(sizeof(int) * pipe_nmbs);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < pipe_nmbs)
	{
		arr[i] = -1;
		i++;
	}
	return (arr);
}

void	create_pipes(t_ppxb *pipex)
{
	int	i;
	int	pairs;

	if (!pipex || !pipex->pipe)
		return ;
	pairs = pipex->cmd_nmbs - 1;
	i = 0;
	while (i < pairs)
	{
		if (pipe(&pipex->pipe[2 * i]) < 0)
		{
			while (i-- > 0)
			{
				if (pipex->pipe[2 * i] >= 0)
					close(pipex->pipe[2 * i]);
				if (pipex->pipe[2 * i + 1] >= 0)
					close(pipex->pipe[2 * i + 1]);
			}
			free_pipes(pipex);
			msg_error(pipex, "pipe");
			return ;
		}
		i++;
	}
}

void	close_pipes(t_ppxb *pipex)
{
	int	i;

	if (!pipex->pipe)
		return ;
	i = 0;
	while (i < pipex->pipe_nmbs)
	{
		if (pipex->pipe[i] >= 0)
			close(pipex->pipe[i]);
		i++;
	}
}

void	free_pipes(t_ppxb *pipex)
{
	if (pipex->pipe)
	{
		free(pipex->pipe);
		pipex->pipe = NULL;
	}
}

void	msg_pipe(char *arg)
{
	write(2, ERR_CMD, ft_strlen(ERR_CMD));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}
