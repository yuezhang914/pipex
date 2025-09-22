/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:34:02 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/19 02:32:55 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char **envp)
{
	if (!envp)
		return (NULL);
	while (*envp && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (!*envp)
		return (NULL);
	return (*envp + 5);
}

void	init_pipex(t_pipex *p, char *progname)
{
	if (!p)
		return ;
	p->infile = -1;
	p->outfile = -1;
	p->tube[0] = -1;
	p->tube[1] = -1;
	p->pid1 = -1;
	p->pid2 = -1;
	p->paths = NULL;
	p->cmd_paths = NULL;
	p->cmd_args = NULL;
	p->cmd = NULL;
	p->progname = progname;
}

void	close_pipes(t_pipex *pipex)
{
	if (!pipex)
		return ;
	if (pipex->tube[0] >= 0)
	{
		close(pipex->tube[0]);
		pipex->tube[0] = -1;
	}
	if (pipex->tube[1] >= 0)
	{
		close(pipex->tube[1]);
		pipex->tube[1] = -1;
	}
}
