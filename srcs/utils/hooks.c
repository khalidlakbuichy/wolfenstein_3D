/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klakbuic <klakbuic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 15:19:43 by klakbuic          #+#    #+#             */
/*   Updated: 2024/07/27 15:27:31 by klakbuic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	key_press(int keycode, t_data *data)
{
	int	movestep;

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
		// free all the allocated memory
		exit(0);
	}
	move_player(data);
	return (0);
}

int	key_realse(int keycode, t_data *data)
{
	if (keycode == UP_ARROW || keycode == DOWN_ARROW || keycode == W
		|| keycode == S)
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