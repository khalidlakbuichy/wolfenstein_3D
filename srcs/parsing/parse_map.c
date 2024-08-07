/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klakbuic <klakbuic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 10:43:10 by klakbuic          #+#    #+#             */
/*   Updated: 2024/08/04 13:15:40 by klakbuic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	is_valide_char_map(char c)
{
	if (c == ' ' || c == '1' || c == '0' || c == 'N' || c == 'S' || c == 'W'
		|| c == 'E' || c == 'D')
		return (true);
	return (false);
}

bool	player_exist(char c)
{
	if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (true);
	return (false);
}

int	max_line_len(t_lst *head)
{
	t_lst	*lst;
	int		max_len;
	int		len;

	lst = head;
	max_len = 0;
	while (lst)
	{
		len = ft_strlen(lst->content);
		if (len > max_len)
			max_len = len;
		lst = (t_lst *)lst->next;
	}
	return (max_len);
}

char	*check_line_map(char *line, bool *player)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if ((size_t) i == ft_strlen(line) - 1 && line[i] == '\n')
		{
			line[i] = '\0';
			break ;
		}
		if (!is_valide_char_map(line[i]))
			ft_error();
		if (player_exist(line[i]) && (*player))
			ft_error();
		if (player_exist(line[i]))
			*player = true;
		i++;
	}
	i = ft_strlen(line) - 1;
	while (i >= 0 && line[i] == ' ')
		line[i--] = '\0';
	return (ft_strdup(line));
}

void	check_map(char **map, int nb_line, int col_len)
{
	int	i;
	int	j;

	i = -1;
	while (++i < nb_line)
	{
		j = -1;
		while (++j < col_len)
		{
			if (map[i][j] == '0' || map[i][j] == '2' || player_exist(map[i][j]))
			{
				if (i == 0 || i == nb_line - 1 || j == 0 || j == col_len - 1)
					ft_error();
				if (map[i - 1][j] == ' ' || map[i + 1][j] == ' ' || map[i][j
					- 1] == ' ' || map[i][j + 1] == ' ')
					ft_error();
			}
			if (map[i][j] == '2')
			{
				if (!((map[i - 1][j] == '1' && map[i + 1][j] == '1') || map[i][j
						- 1] == '1' || map[i][j + 1] == '1'))
					ft_error();
			}
		}
	}
}
