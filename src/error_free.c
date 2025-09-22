/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:51:30 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/17 00:22:00 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	msg(t_pipex *p, const char *err)
{
	if (!err)
		return (1);
	if (p && p->progname)
	{
		write(STDERR_FILENO, p->progname, ft_strlen(p->progname));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, err, ft_strlen((char *)err));
	return (1);
}

void	msg_error(t_pipex *p, const char *target)
{
	if (p && p->progname)
	{
		write(STDERR_FILENO, p->progname, ft_strlen(p->progname));
		write(STDERR_FILENO, ": ", 2);
	}
	perror(target);
	exit(1);
}

void	parent_free(t_pipex *pipex)
{
	int	i;

	if (!pipex)
		return ;
	if (pipex->infile >= 0)
	{
		close(pipex->infile);
		pipex->infile = -1;
	}
	if (pipex->outfile >= 0)
	{
		close(pipex->outfile);
		pipex->outfile = -1;
	}
	close_pipes(pipex);
	if (pipex->cmd_paths)
	{
		i = 0;
		while (pipex->cmd_paths[i])
		{
			free(pipex->cmd_paths[i++]);
		}
		free(pipex->cmd_paths);
		pipex->cmd_paths = NULL;
	}
}

void	child_free(t_pipex *pipex)
{
	int	i;

	if (!pipex)
		return ;
	if (pipex->cmd_args)
	{
		i = 0;
		while (pipex->cmd_args[i])
		{
			free(pipex->cmd_args[i]);
			pipex->cmd_args[i] = NULL;
			i++;
		}
		free(pipex->cmd_args);
		pipex->cmd_args = NULL;
	}
	if (pipex->cmd)
	{
		free(pipex->cmd);
		pipex->cmd = NULL;
	}
}
