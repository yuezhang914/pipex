/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:33:31 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/13 01:27:46 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_split_error(t_pipex *pipex)
{
	child_free(pipex);
	msg(pipex, "Error: ft_split failed\n");
	exit(1);
}

static void	cmd_not_found(t_pipex *pipex)
{
	child_free(pipex);
	msg(pipex, "Error: command not found\n");
	exit(127);
}

static void	execve_fail_exit(t_pipex *pipex, int saved_errno)
{
	child_free(pipex);
	errno = saved_errno;
	perror("execve");
	exit(126);
}

void	first_child(t_pipex pipex, char *argv[], char *envp[])
{
	int	saved_errno;

	if (dup2(pipex.tube[1], STDOUT_FILENO) == -1)
		msg_error(&pipex, "dup2");
	close(pipex.tube[1]);
	close(pipex.tube[0]);
	if (dup2(pipex.infile, STDIN_FILENO) == -1)
		msg_error(&pipex, "dup2");
	close(pipex.infile);
	pipex.cmd_args = ft_split(argv[2], ' ');
	if (!pipex.cmd_args)
		ft_split_error(&pipex);
	pipex.cmd = get_cmd(pipex.cmd_paths, pipex.cmd_args[0]);
	if (!pipex.cmd)
		cmd_not_found(&pipex);
	execve(pipex.cmd, pipex.cmd_args, envp);
	saved_errno = errno;
	execve_fail_exit(&pipex, saved_errno);
}

void	second_child(t_pipex pipex, char *argv[], char *envp[])
{
	int	saved_errno;

	if (dup2(pipex.tube[0], STDIN_FILENO) == -1)
		msg_error(&pipex, "dup2");
	close(pipex.tube[0]);
	close(pipex.tube[1]);
	if (dup2(pipex.outfile, STDOUT_FILENO) == -1)
		msg_error(&pipex, "dup2");
	close(pipex.outfile);
	pipex.cmd_args = ft_split(argv[3], ' ');
	if (!pipex.cmd_args)
		ft_split_error(&pipex);
	pipex.cmd = get_cmd(pipex.cmd_paths, pipex.cmd_args[0]);
	if (!pipex.cmd)
		cmd_not_found(&pipex);
	execve(pipex.cmd, pipex.cmd_args, envp);
	saved_errno = errno;
	execve_fail_exit(&pipex, saved_errno);
}
