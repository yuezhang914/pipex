/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:57:32 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/18 00:02:46 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	args_in(int argc, char **argv, t_ppxb *pipex)
{
	if (argc < 2)
		return (0);
	if (argv[1] && !ft_strncmp("here_doc", argv[1], 9))
	{
		pipex->here_doc = 1;
		return (6);
	}
	pipex->here_doc = 0;
	return (5);
}

void	get_infile(int argc, char **argv, t_ppxb *pipex)
{
	(void)argc;
	if (pipex->here_doc)
	{
		pipex->infile = do_here_doc(argv[2]);
		if (pipex->infile < 0)
		{
			msg_error(pipex, "here_doc");
		}
	}
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		if (pipex->infile < 0)
		{
			msg_error(pipex, argv[1]);
		}
	}
}

void	get_outfile(char *argv_last, t_ppxb *pipex)
{
	if (pipex->here_doc)
		pipex->outfile = open(argv_last, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipex->outfile = open(argv_last, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex->outfile < 0)
		msg_error(pipex, argv_last);
}

char	*find_path(char **envp)
{
	while (envp && *envp)
	{
		if (!ft_strncmp("PATH=", *envp, 5))
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}
