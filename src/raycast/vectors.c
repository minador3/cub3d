/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:16:00 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_step_x(t_player *player, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - player->x) * ray->delta_dist_x;
	}
}

static void	init_step_y(t_player *player, t_ray *ray)
{
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->y) * ray->delta_dist_y;
	}
}

void	init_dda(t_player *player, t_ray *ray)
{
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
	init_step_x(player, ray);
	init_step_y(player, ray);
}

void	calc_wall_height(t_ray *ray, t_wall *wall, int screen_height)
{
	if (ray->side == 0)
		wall->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
	else
		wall->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;
	if (wall->perp_wall_dist < 1e-6)
		wall->perp_wall_dist = 1e-6;
	wall->line_height = (int)(screen_height / wall->perp_wall_dist);
	wall->draw_start = (screen_height / 2) - (wall->line_height / 2);
	wall->draw_end = (screen_height / 2) + (wall->line_height / 2);
	if (wall->draw_start < 0)
		wall->draw_start = 0;
	if (wall->draw_end >= screen_height)
		wall->draw_end = screen_height - 1;
}

void	dda_loop(t_ray *ray, t_game *game)
{
	while (1)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->map_x += ray->step_x;
			ray->side_dist_x += ray->delta_dist_x;
			ray->side = 0;
		}
		else
		{
			ray->map_y += ray->step_y;
			ray->side_dist_y += ray->delta_dist_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_y >= game->map.h
			|| ray->map_x < 0 || ray->map_x >= game->map.w)
			break ;
		if (game->map.grid[ray->map_y][ray->map_x] == '1')
			break ;
	}
}
