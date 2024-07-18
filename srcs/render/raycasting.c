/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 04:14:03 by soutchak          #+#    #+#             */
/*   Updated: 2024/07/18 06:10:04 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define FOV (60 * (PI / 180)) // 60 degrees in radians
#define NUM_RAYS (WIDTH / 100)

double  normalizeAngle(double angle)
{
    double new;

    new = fmod(angle, 2 * PI);
    if (new < 0)
        new += 2 * PI;
    return new;
}

t_point hor_intersection_distance(t_ray ray, t_player *player)
{
    t_point point;
    double  xIntercept;
    double  yIntercept;
    double  xStep;
    double  yStep;

    // find horizontal (y/x)(inter/step) values
    yIntercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
    if (!ray.facingUp)
        yIntercept += TILE_SIZE;
    xIntercept = player->x + ((yIntercept - player->y) / tan(ray.angle));
    yStep = TILE_SIZE;
    if (ray.facingUp)
        yStep *= -1;
    xStep = yStep / tan(ray.angle);
    if ((xStep > 0 && !ray.facingRight) || (xStep < 0 && ray.facingRight))
        xStep *= -1;

    // find horizontal intersection
    point.x = xIntercept;
    point.y = yIntercept;
    if (ray.facingUp)
        point.y--;
    while ((point.x > 0 && point.x < WIDTH) && (point.y > 0 && point.y < HEIGHT))
    {
        if (is_wall(point.x, point.y, get_data(NULL)))
            break ;
        else
        {
            point.x += xStep;
            point.y += yStep;
        }
    }
    return point;
}

void cast_rays(t_mlx *mlx, t_player *player) {
    double rayangle = player->rotationAngle - FOV / 2;
    double angle_inc = FOV / (double)NUM_RAYS;

    t_ray   ray[NUM_RAYS];

    // printf("Player Angle: %f, Initial Ray Angle: %f\n", player->rotationAngle, rayangle);
    // printf("Angle Increment: %f\n", angle_inc);

    for (int i = 0; i < NUM_RAYS; i++) {
    // for (int i = 0; i < 1; i++) {
        // ray[i].angle = normalizeAngle(rayangle);
        rayangle = normalizeAngle(rayangle);
        ray[i].angle = rayangle;
        if (ray[i].angle > PI)
            ray[i].facingUp = true;
        else
            ray[i].facingUp = false;
        if (ray[i].angle < (PI / 2) || ray[i].angle > ((3 * PI) / 2))
            ray[i].facingRight = true;
        else
            ray[i].facingRight = false;

        printf(" ===========> Ray facingUp: %d facingRight: %d ==========>\n", ray[i].facingUp, ray[i].facingRight);

        t_point horInter = hor_intersection_distance(ray[i], player);
        ft_draw_line(mlx, player->x, player->y, horInter.x, horInter.y, 0x00000000);
        
        double ray_x = player->x + cos(rayangle) * 50;
        double ray_y = player->y + sin(rayangle) * 50;
    
        double sideYhor = floor(player->y / TILE_SIZE) * TILE_SIZE;
        double sideXhor = player->x + ((player->y - sideYhor) / tan(rayangle));
        double deltaYhor = TILE_SIZE;
        double deltaXhor = deltaYhor / tan(rayangle);

        double sideXver = floor(player->x / TILE_SIZE) * TILE_SIZE;
        // double sideYver = floor(player->y / TILE_SIZE) * TILE_SIZE;
        double deltaXver = TILE_SIZE;
        double deltaYver = deltaXver * tan(rayangle);

        // printf("Ray %d: angle %f, end point (%f, %f)\n", i, rayangle, ray_x, ray_y);
        // printf("==> sideY: %f sideX: %f deltaY: %f deltaX: %f\n\n", sideYhor, sideXhor, deltaYhor, deltaXhor);

        // ft_draw_line(mlx, player->x, player->y, ray_x, ray_y, 0x00000080);

        rayangle += angle_inc;
        // rayangle = normalizeAngle(rayangle);
    }
}
