/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 23:44:37 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 02:58:51 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	wait_for_pid(pid_t pid, int *status)
{
	pid_t	w;

	while (1)
	{
		w = waitpid(pid, status, 0);
		if (w == -1 && errno == EINTR)
			continue ;
		break ;
	}
	if (w == -1)
	{
		if (errno == ECHILD)
			return (1);
		return (-1);
	}
	return (0);
}

int	wait_by_pids(t_ppxb *pipex)
{
	int	status;
	int	i;
	int	rc;
	int	exit_code;

	exit_code = 0;
	i = 0;
	while (i < pipex->cmd_nmbs)
	{
		rc = wait_for_pid(pipex->pids[i], &status);
		if (rc == -1)
		{
			perror("waitpid");
			exit_code = 1;
		}
		else if (rc == 1)
			exit_code = 1;
		else if (pipex->pids[i] == pipex->pids[pipex->cmd_nmbs - 1])
			exit_code = get_exit_code(status);
		i = i + 1;
	}
	return (exit_code);
}

static int	wait_any_child(int *out_exit_code)
{
	int		status;
	int		ec;
	pid_t	w;

	status = 0;
	ec = 0;
	while (1)
	{
		w = wait(&status);
		if (w > 0)
		{
			ec = get_exit_code(status);
			continue ;
		}
		break ;
	}
	if (w == -1 && errno != ECHILD)
		return (-1);
	*out_exit_code = ec;
	return (0);
}

int	wait_and_cleanup(t_ppxb *pipex)
{
	int	exit_code;

	exit_code = 0;
	if (!pipex)
		return (1);
	if (pipex->pids)
		return (wait_by_pids(pipex));
	if (wait_any_child(&exit_code) < 0)
		return (1);
	return (exit_code);
}
