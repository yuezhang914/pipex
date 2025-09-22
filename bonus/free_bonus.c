/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:59:23 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 03:00:44 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	free_cmd_paths(t_ppxb *pipex)
{
	int	i;

	if (!pipex || !pipex->cmd_paths)
		return ;
	i = 0;
	while (pipex->cmd_paths[i])
	{
		free(pipex->cmd_paths[i]);
		i++;
	}
	free(pipex->cmd_paths);
	pipex->cmd_paths = NULL;
}

void	parent_free(t_ppxb *pipex)
{
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
	if (pipex->pids)
	{
		free(pipex->pids);
		pipex->pids = NULL;
	}
	free_cmd_paths(pipex);
	close_pipes(pipex);
	free_pipes(pipex);
}

void	child_free(t_ppxb *pipex)
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
