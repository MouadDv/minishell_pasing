/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbensarg <sbensarg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 04:34:45 by milmi             #+#    #+#             */
/*   Updated: 2021/11/15 18:37:04 by sbensarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sighandler(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		write (1, "\033[32;1mminishell$ \033[0m", 23);
		write (1, rl_line_buffer, ft_strlen(rl_line_buffer));
		write (1, "  \b\b\n", 5);
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	get_line(t_node *node)
{
	char	*buf;

	buf = readline("\033[32;1mminishell$ \033[0m");
	if (buf == NULL)
	{
		write(1, "exit\n", 5);
		free_node(node);
		// if (g_data.path != NULL)
		// 	free_ptrs(g_data.path);
		// if(g_data.newpath != NULL)
		// 	free(g_data.newpath);
		// //free(g_data.cmderr);
		// free(g_data.tab);
		// free_ptrs(g_data.ptrs);
		exit(0);
	}
	if (ft_strlen(buf) > 0)
	{
		add_history(buf);
		if (scan(buf) == 0)
			write (1, "Minishell: Syntax error\n", 24);
		else
			parse_and_exec(buf, node);
	}
	free(buf);
}

void	ft_init(t_node *head)
{
	t_red *tmp2;

	g_data.path = ft_path(head);
	g_data.newpath = NULL;
	g_data.cmderr = malloc(sizeof(tmp2->arg));
	if (!g_data.cmderr)
		return ;
	g_data.tab = malloc(sizeof(int) * 3);
	if (!g_data.tab)
		return ;
	g_data.ptrs = malloc(sizeof(t_cmd));
	if (!g_data.ptrs)
		return ;
}

int	main(int argc, char **argv, char **env)
{
	t_node	*node;
	char	**newenv;
	int		i;

	node = NULL;
	i = 0;
	if (argc == 1)
	{
		ft_strlen(argv[0]);
		newenv = copy_env(env);
		init_struct(newenv, &node);
		ft_init(node);
		while (newenv[i++])
			free_null(newenv[i - 1]);
		free_null(newenv);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sighandler);
		while (1)
			get_line(node);
		return (0);
	}
	return (1);
}

char	*get_name(char	*str)
{
	char	*ret;
	int		i;

	i = 0;
	if (str[i] == '?')
		return (ft_strdup("?"));
	while (str[i] && ((str[i] >= '0' && str[i] <= '9')
			|| (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| str[i] == '_'))
		i++;
	ret = ft_substr(str, 0, i);
	return (ret);
}
