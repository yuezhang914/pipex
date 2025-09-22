/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 01:47:14 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/19 01:48:12 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	open_input_output(t_pipex *pipex, char *infile, char *outfile)
{
	pipex->infile = open(infile, O_RDONLY);
	if (pipex->infile < 0)
	{
		parent_free(pipex);
		msg_error(pipex, infile);
	}
	pipex->outfile = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
	{
		parent_free(pipex);
		msg_error(pipex, outfile);
	}
}

static void	create_pipe_or_exit(t_pipex *pipex)
{
	if (pipe(pipex->tube) < 0)
	{
		parent_free(pipex);
		msg_error(pipex, "pipe");
	}
}

static int	prepare_paths(t_pipex *pipex, char *envp[])
{
	pipex->paths = find_path(envp);
	if (!pipex->paths)
	{
		parent_free(pipex);
		return (msg(pipex, "Error: PATH not found\n"));
	}
	pipex->cmd_paths = ft_split(pipex->paths, ':');
	if (!pipex->cmd_paths)
	{
		parent_free(pipex);
		return (msg(pipex, "Error: ft_split failed\n"));
	}
	return (0);
}

static void	fork_children(t_pipex *pipex, char *argv[], char *envp[])
{
	pipex->pid1 = fork();
	if (pipex->pid1 < 0)
	{
		parent_free(pipex);
		msg_error(pipex, "fork");
	}
	if (pipex->pid1 == 0)
		first_child(*pipex, argv, envp);
	pipex->pid2 = fork();
	if (pipex->pid2 < 0)
	{
		parent_free(pipex);
		msg_error(pipex, "fork");
	}
	if (pipex->pid2 == 0)
		second_child(*pipex, argv, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;
	int		status1;
	int		status2;
	int		ret;

	init_pipex(&pipex, argv[0]);
	if (argc != 5)
		return (msg(&pipex, "Error: Bad arguments\n"));
	open_input_output(&pipex, argv[1], argv[argc - 1]);
	create_pipe_or_exit(&pipex);
	if (prepare_paths(&pipex, envp) != 0)
		return (1);
	fork_children(&pipex, argv, envp);
	close_pipes(&pipex);
	waitpid(pipex.pid1, &status1, 0);
	waitpid(pipex.pid2, &status2, 0);
	if (WIFEXITED(status2))
		ret = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		ret = 128 + WTERMSIG(status2);
	else
		ret = 1;
	parent_free(&pipex);
	return (ret);
}
