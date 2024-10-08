/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klakbuic <klakbuic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:43:36 by klakbuic          #+#    #+#             */
/*   Updated: 2024/08/04 13:13:41 by klakbuic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_data	*get_data(t_data *data)
{
	static t_data	*tmp = NULL;

	if (data)
		tmp = data;
	return (tmp);
}

void	ft_init_mlx(t_mlx *mlx)
{
	mlx->mlx_ptr = mlx_init();
	if (NULL == mlx->mlx_ptr)
		free(mlx);
	mlx->win = mlx_new_window(mlx->mlx_ptr, WIDTH, HEIGHT, "cub3D");
	if (NULL == mlx->win)
	{
		mlx_destroy_display(mlx->mlx_ptr);
		free(mlx);
	}
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, WIDTH, HEIGHT);
	if (NULL == mlx->img.img_ptr)
	{
		mlx_destroy_window(mlx->mlx_ptr, mlx->win);
		mlx_destroy_display(mlx->mlx_ptr);
		free(mlx);
	}
	mlx->img.addr = mlx_get_data_addr(mlx->img.img_ptr, &mlx->img.bpp,
			&mlx->img.line_length, &mlx->img.endian);
	mlx->img.width = WIDTH;
	mlx->img.height = HEIGHT;
}

void	ft_init_player_position(t_player *player, t_map_data *map_data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < map_data->map.rows)
	{
		j = -1;
		while (++j < map_data->map.cols)
		{
			if (is_player(map_data->map.map[i][j]))
			{
				player->x = (j * TILE_SIZE) + (TILE_SIZE / 2);
				player->y = (i * TILE_SIZE) + (TILE_SIZE / 2);
				if (map_data->map.map[i][j] == 'N')
					player->rotation_angle = 3 * PI / 2;
				else if (map_data->map.map[i][j] == 'S')
					player->rotation_angle = PI / 2;
				else if (map_data->map.map[i][j] == 'E')
					player->rotation_angle = 0;
				else if (map_data->map.map[i][j] == 'W')
					player->rotation_angle = PI;
				return ;
			}
		}
	}
}

void	ft_init_data(t_data *data)
{
	t_player	*player;
	t_map_data	*map_data;

	player = data->player;
	map_data = data->map_data;
	map_data->so_texture = NULL;
	map_data->no_texture = NULL;
	map_data->we_texture = NULL;
	map_data->ea_texture = NULL;
	map_data->ceil_color = -1;
	map_data->floor_color = -1;
	player->radius = 100;
	player->turn_direction = 0;
	player->walk_direction = 0;
	player->hor_move = 0;
	player->walk_speed = 0.05 * TILE_SIZE;
	player->turen_speed = 2 * (PI / 180);
}

void	ft_init_lock(t_data *data)
{
	data->quit.flag = false;
	if (pthread_mutex_init(&data->quit.mutex, NULL) != 0)
		return (printf(RED "Error init mutex\n" RESET), exit(1));
}
