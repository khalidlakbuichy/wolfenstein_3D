/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klakbuic <klakbuic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:46:50 by klakbuic          #+#    #+#             */
/*   Updated: 2024/07/27 15:10:04 by klakbuic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ft_draw_square(t_mlx *mlx, int x, int y, int size, int color)
{
	for (int i = x; i < x + size; i++)
	{
		for (int j = y; j < y + size; j++)
		{
			int index = (j * WIDTH + i) * 4;
			mlx->img.addr[index] = color & 0xFF;
			mlx->img.addr[index + 1] = (color & 0xFF00) >> 8;
			mlx->img.addr[index + 2] = (color & 0xFF0000) >> 16;
		}
	}
}

void ft_draw_line(t_mlx *mlx, int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

	while (1) {
    if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
        int index = (y0 * mlx->img.line_length + x0 * (mlx->img.bpp / 8));
        // printf("Drawing pixel at (%d, %d) with index %d\n", x0, y0, index);
        mlx->img.addr[index] = color;
        mlx->img.addr[index + 1] = color >> 8;
        mlx->img.addr[index + 2] = color >> 16;
    }

    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
	}
}

void ft_draw_circle(t_mlx *mlx, int xc, int yc, int radius, int color) {
    int x = radius;
    int y = 0;
    int p = 1 - radius;

    // Draw initial points on each quadrant
    ft_draw_line(mlx, xc - x, yc, xc + x, yc, color); // Initial horizontal diameter

    if (radius > 0) {
        ft_draw_square(mlx, xc - radius, yc, 1, color); // Leftmost point
        ft_draw_square(mlx, xc + radius, yc, 1, color); // Rightmost point
        ft_draw_square(mlx, xc, yc + radius, 1, color); // Topmost point
        ft_draw_square(mlx, xc, yc - radius, 1, color); // Bottommost point
    }

    // Filling the circle
    while (x > y) {
        y++;

        // Mid-point is inside or on the perimeter
        if (p <= 0) {
            p = p + 2*y + 1;
        } else { // Mid-point is outside the perimeter
            x--;
            p = p + 2*y - 2*x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y) break;

        // Drawing the horizontal lines for each quadrant
        ft_draw_line(mlx, xc - x, yc + y, xc + x, yc + y, color);
        ft_draw_line(mlx, xc - x, yc - y, xc + x, yc - y, color);

        if (x != y) {
            ft_draw_line(mlx, xc - y, yc + x, xc + y, yc + x, color);
            ft_draw_line(mlx, xc - y, yc - x, xc + y, yc - x, color);
        }
    }
}

void	ft_render_player(t_mlx *mlx, t_player *player)
{
	// printf("===> player.x: %f, player.y: %f\n", player->x, player->y);
	cast_rays(mlx, player);
}

void ft_render_map(t_mlx *mlx, t_map_data *map_data)
{
		for (int i = 0; i < map_data->map.rows; i++)
	{
		for (int j = 0; j < map_data->map.cols; j++)
		{
			if (map_data->map.map[i][j] == '1')
				ft_draw_square(mlx,
								j * TILE_SIZE * MINIMAP_SCALE,
								i * TILE_SIZE * MINIMAP_SCALE,
								TILE_SIZE * MINIMAP_SCALE,
								0x00FF0000);
			else if (map_data->map.map[i][j] == '0')
				ft_draw_square(mlx,
								j * TILE_SIZE * MINIMAP_SCALE,
								i * TILE_SIZE * MINIMAP_SCALE,
								TILE_SIZE * MINIMAP_SCALE,
								0x00FFFFFF);
		}
	}
}

bool	cannot_move(double x, double y, t_data *data)
{
	int	playerSize = 30;

	return (
		is_wall_1(x - playerSize, y - playerSize, data) ||
		is_wall_1(x + playerSize, y - playerSize, data) ||
		is_wall_1(x - playerSize, y + playerSize, data) ||
		is_wall_1(x + playerSize, y + playerSize, data)
	);
}

bool is_wall_2(double x, double y, t_data *data)
{
	int	mapX, mapY;

	mapY = (int)floor((double)y / (double)TILE_SIZE);
	mapX = (int)floor((double)x / (double)TILE_SIZE);
	if ((mapY < 0 || mapY >= data->map_data->map.rows) || (mapX < 0 || mapX >= data->map_data->map.cols))
	{
		printf("========> EDGE\n");
		return (true);
	}
	if (data->map_data->map.map[mapY][mapX] == '1')
		return (true);
	else
		return (false);
}

bool is_wall_1(double x, double y, t_data *data)
{
	int	mapX, mapY;

	mapY = (int)floor((double)y / (double)(TILE_SIZE + EPSILON));
	mapX = (int)floor((double)x / (double)(TILE_SIZE + EPSILON));
	if ((mapY < 0 || mapY >= data->map_data->map.rows) || (mapX < 0 || mapX >= data->map_data->map.cols))
	{
		printf("========> EDGE\n");
		return (true);
	}
	if (data->map_data->map.map[mapY][mapX] == '1')
		return (true);
	else
		return (false);
}

void render_image(t_data *data)
{
	mlx_destroy_image(data->mlx->mlx_ptr, data->mlx->img.img_ptr);
	data->mlx->img.img_ptr = mlx_new_image(data->mlx->mlx_ptr, WIDTH, HEIGHT);
	ft_render_map(data->mlx, data->map_data);
	ft_render_player(data->mlx, data->player);
	mlx_put_image_to_window(data->mlx->mlx_ptr, data->mlx->win, data->mlx->img.img_ptr, 0, 0);
}

void move_player(t_data *data)
{
	t_player *player;
	double movestep;
	double next_y;
	double next_x;
	
	player = data->player;
	movestep = player->walkDirection * player->walkSpeed;
	player->rotationAngle += player->turnDirection * player->turnSpeed;
	next_x = player->x + cos(player->rotationAngle + player->horMove) * movestep;
	next_y = player->y + sin(data->player->rotationAngle + player->horMove) * movestep;
	if (!cannot_move(next_x, next_y, data))
	{
		data->player->x = next_x;
		data->player->y = next_y;
		render_image(data);
	}
}


int key_press(int keycode, t_data *data)
{
	int movestep;
	float next_y, next_x;

	if (keycode == UP_ARROW || keycode == W)
	{
		data->player->walkDirection = 1;
	}
	else if (keycode == DOWN_ARROW || keycode == S)
	{
		data->player->walkDirection = -1;
	}
	else if (keycode == LEFT_ARROW)
	{
		data->player->turnDirection = -1;
	}
	else if (keycode == RIGHT_ARROW)
	{
		data->player->turnDirection = 1;
	}
	 else if (keycode == A)
	{
		data->player->walkDirection = -1; // Move left
		data->player->horMove = PI_2;
	}
	else if (keycode == D)
	{
		data->player->walkDirection = 1; // Move right
		data->player->horMove = PI_2;
	}
	else if (keycode == ESC)
	{
		exit(0);
	}
	move_player(data);
	return (0);
}

int key_realse(int keycode, t_data *data)
{
	if (keycode == UP_ARROW || keycode == DOWN_ARROW || keycode == W || keycode == S)
	{
		data->player->walkDirection = 0;
	}
	else if (keycode == LEFT_ARROW || keycode == RIGHT_ARROW)
	{
		data->player->turnDirection = 0;
		data->player->horMove = 0;
	}
	else if (keycode == A || keycode == D)
	{
		data->player->horMove = 0;
	}
	return (0);
}

t_data	*get_data(t_data *data)
{
	static t_data *tmp = NULL;

	if (data)
		tmp = data;
	return (tmp);
}

void	load_textures(t_map_data *map_data, t_mlx *mlx)
{
	int	width;
	int	height;

	printf("Loading textures\n");
	printf("NO: |%s|\n", map_data->no_texture);
	printf("SO: |%s|\n", map_data->so_texture);
	printf("WE: |%s|\n", map_data->we_texture);
	printf("EA: |%s|\n", map_data->ea_texture);
	// // Calculate the number of pixels
    // int num_pixels = width * height;
    // int bytes_per_pixel = map_data->no_texture_img.bpp / 8;

    // // Print pixel data
    // for (int i = 0; i < num_pixels; i++) {
    //     int pixel_offset = i * bytes_per_pixel;
    //     unsigned int color = *(unsigned int *)(map_data->no_texture_img.addr + pixel_offset);
    //     printf("NO[%d]: %#x\n", i, color);
    // }
	// sleep(5);
	
	map_data->no_texture_img.img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, map_data->no_texture, &map_data->no_texture_img.width, &map_data->no_texture_img.height);
	map_data->no_texture_img.addr = mlx_get_data_addr(map_data->no_texture_img.img_ptr, &map_data->no_texture_img.bpp, &map_data->no_texture_img.line_length, &map_data->no_texture_img.endian);
	printf(" ===================== 1111111 =====================\n");

	map_data->so_texture_img.img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, map_data->so_texture, &map_data->so_texture_img.width, &map_data->so_texture_img.height);
	map_data->so_texture_img.addr = mlx_get_data_addr(map_data->so_texture_img.img_ptr, &map_data->so_texture_img.bpp, &map_data->so_texture_img.line_length, &map_data->so_texture_img.endian);
	printf(" ===================== 222222 =====================\n");

	map_data->we_texture_img.img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, map_data->we_texture, &map_data->we_texture_img.width, &map_data->we_texture_img.height);
	map_data->we_texture_img.addr = mlx_get_data_addr(map_data->we_texture_img.img_ptr, &map_data->we_texture_img.bpp, &map_data->we_texture_img.line_length, &map_data->we_texture_img.endian);
	printf(" ===================== 333333 =====================\n");

	map_data->ea_texture_img.img_ptr = mlx_xpm_file_to_image(mlx->mlx_ptr, map_data->ea_texture, &map_data->ea_texture_img.width, &map_data->ea_texture_img.height);
	map_data->ea_texture_img.addr = mlx_get_data_addr(map_data->ea_texture_img.img_ptr, &map_data->ea_texture_img.bpp, &map_data->ea_texture_img.line_length, &map_data->ea_texture_img.endian);
	printf(" ===================== 44444 =====================\n");

	printf("Done loading textures\n");
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Error\nInvalid number of arguments\n");
		return (EXIT_FAILURE);
	}
	t_map_data map_data;
	t_player player;
	t_mlx mlx;
	t_data data;
	data.mlx = &mlx;
	data.map_data = &map_data;
	data.player = &player;

	get_data(&data);

	// INIT:
	map_data.so_texture = NULL;
	map_data.no_texture = NULL;
	map_data.we_texture = NULL;
	map_data.ea_texture = NULL;
	map_data.ceil_color = -1;
	map_data.floor_color = -1;
	// INIT:
	parse_map_file(av[1], &map_data);
	ft_init_mlx(&mlx);
	load_textures(&map_data, &mlx);
	// Render map:
	ft_render_map(&mlx, &map_data);

	// Render player:
	player.x = WIDTH / 8;
	player.y = HEIGHT / 5 + 30;
	player.x = 2 * TILE_SIZE;
	player.y = 2 * TILE_SIZE;

	player.radius = 7;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2;
	player.horMove = 0;
	// player.walkSpeed = 10 * (TILE_SIZE / 100);
	player.walkSpeed = 0.15 * TILE_SIZE;
	// player.walkSpeed = 10;
	player.turnSpeed = 10 * (PI / 180);
	ft_render_player(&mlx, &player);
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win, mlx.img.img_ptr, 0, 0);
	mlx_hook(mlx.win, 2, 1L << 0, key_press, &data);
	mlx_hook(mlx.win, 3, 1L << 1, key_realse, &data);
	mlx_loop(mlx.mlx_ptr);
}