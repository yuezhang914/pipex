/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:59:51 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/19 03:35:52 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_pipex(t_ppxb *p)
{
	p->infile = -1;
	p->outfile = -1;
	p->pipe = NULL;
	p->cmd_nmbs = 0;
	p->pipe_nmbs = 0;
	p->here_doc = 0;
	p->env_path = NULL;
	p->cmd_paths = NULL;
	p->cmd_args = NULL;
	p->cmd = NULL;
	p->idx = 0;
	p->pids = NULL;
}

int	msg(t_ppxb *pipex, const char *err)
{
	if (err)
		write(2, err, ft_strlen(err));
	if (pipex)
		parent_free(pipex);
	return (1);
}

void	msg_error(t_ppxb *pipex, const char *target)
{
	if (target)
	{
		write(2, "pipex: ", 7);
		if (errno)
		{
			perror(target);
		}
		else
		{
			write(2, target, strlen(target));
			write(2, ": error\n", 8);
		}
	}
	else
	{
		if (errno)
			perror("pipex");
		else
			write(2, "pipex: error\n", 13);
	}
	if (pipex)
		parent_free(pipex);
	exit(1);
}
