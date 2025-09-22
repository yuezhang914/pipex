/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:58:52 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 05:12:07 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	exec_all_cmds(t_ppxb *pipex, char **argv, char **envp)
{
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_nmbs);
	if (!pipex->pids)
		msg_error(pipex, "malloc");
	pipex->idx = 0;
	while (pipex->idx < pipex->cmd_nmbs)
	{
		pipex->pids[pipex->idx] = fork();
		if (pipex->pids[pipex->idx] < 0)
			msg_error(pipex, "fork");
		if (pipex->pids[pipex->idx] == 0)
			exec_child(pipex, argv, envp);
		pipex->idx++;
	}
}

static void	set_child_io(t_ppxb *p)
{
	int	in_fd;
	int	out_fd;

	if (p->idx == 0)
		in_fd = p->infile;
	else
		in_fd = p->pipe[2 * (p->idx - 1)];
	if (p->idx == p->cmd_nmbs - 1)
		out_fd = p->outfile;
	else
		out_fd = p->pipe[2 * p->idx + 1];
	if (dup2(in_fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close_pipes(p);
		child_free(p);
		exit(1);
	}
	if (dup2(out_fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close_pipes(p);
		child_free(p);
		exit(1);
	}
}

static void	run_cmd(t_ppxb *p, char **argv, char **envp)
{
	char	*argstr;
	int		offset;

	offset = 2 + p->here_doc + p->idx;
	argstr = argv[offset];
	p->cmd_args = ft_split(argstr, ' ');
	if (!p->cmd_args || !p->cmd_args[0])
	{
		msg_pipe(argstr);
		child_free(p);
		exit(127);
	}
	p->cmd = get_cmd(p->cmd_paths, p->cmd_args[0]);
	if (!p->cmd)
	{
		msg_pipe(p->cmd_args[0]);
		child_free(p);
		exit(127);
	}
	execve(p->cmd, p->cmd_args, envp);
	perror("execve");
	child_free(p);
	exit(126);
}

void	exec_child(t_ppxb *pipex, char **argv, char **envp)
{
	set_child_io(pipex);
	close_pipes(pipex);
	run_cmd(pipex, argv, envp);
}
