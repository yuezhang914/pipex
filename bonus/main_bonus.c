/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 01:47:56 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 02:42:24 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	prepare_io_and_counts(t_ppxb *p, int argc, char **argv)
{
	get_infile(argc, argv, p);
	get_outfile(argv[argc - 1], p);
	p->cmd_nmbs = argc - 3 - p->here_doc;
	if (p->cmd_nmbs <= 0)
		return (msg(p, ERR_INPUT));
	p->pipe_nmbs = 2 * (p->cmd_nmbs - 1);
	if (p->pipe_nmbs > 0)
	{
		p->pipe = alloc_pipes(p->pipe_nmbs);
		if (!p->pipe)
			return (msg(p, "malloc"));
	}
	return (0);
}

static int	setup_paths_env(t_ppxb *p, char **envp)
{
	p->env_path = find_path(envp);
	if (!p->env_path)
		return (msg(p, ERR_ENVP));
	p->cmd_paths = ft_split(p->env_path, ':');
	if (!p->cmd_paths)
		return (msg(p, "ft_split"));
	create_pipes(p);
	return (0);
}

static int	setup_pipex(t_ppxb *p, int argc, char **argv, char **envp)
{
	int	min_args;
	int	rc;

	init_pipex(p);
	min_args = args_in(argc, argv, p);
	if (min_args == 0 || argc < min_args)
		return (msg(NULL, ERR_INPUT));
	rc = prepare_io_and_counts(p, argc, argv);
	if (rc != 0)
		return (rc);
	return (setup_paths_env(p, envp));
}

static int	run_and_cleanup(t_ppxb *p, char **argv, char **envp)
{
	int	rc;

	exec_all_cmds(p, argv, envp);
	close_pipes(p);
	rc = wait_and_cleanup(p);
	parent_free(p);
	return (rc);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ppxb	pipex;
	int		rc;

	rc = setup_pipex(&pipex, argc, argv, envp);
	if (rc != 0)
		return (rc);
	return (run_and_cleanup(&pipex, argv, envp));
}
