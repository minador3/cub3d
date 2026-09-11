#include "cub3d.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int colour)
{
	int offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->addr + offset) = colour;
}

void	paint_ceiling_floor(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->screen.h)
	{
		x = 0;
		if (y < game->screen.h / 2)
		{
			while (x < game->screen.w)
			{
				my_mlx_pixel_put(&game->screen, x, y, game->map.ceil_hex);
				x++;
			}
		}
		else
		{
			while (x < game->screen.w)
			{
				my_mlx_pixel_put(&game->screen, x, y, game->map.floor_hex);
				x++;
			}
		}
		y++;
	}
}

t_img	*select_texture(t_ray *ray, t_tex *tex)
{
	if (ray->side == 0 && ray->step_x > 0)
		return (&tex->we_img);
	else if (ray->side == 0)
		return (&tex->ea_img);
	else if (ray->step_y > 0)
		return (&tex->no_img);
	else
		return (&tex->so_img);
}

void	paint_wall_column(t_game *game, int x, t_ray *ray, t_wall *wall)
{
	int		y;
	int		tex_y;
	int		color;
	t_img	*tex;

	if (ray->side == 0)
		wall->wall_x = game->player.y + wall->perp_wall_dist * ray->ray_dir_y;
	else
		wall->wall_x = game->player.x + wall->perp_wall_dist * ray->ray_dir_x;
	wall->wall_x -= floor(wall->wall_x);
	tex = select_texture(ray, &game->tex);
	wall->tex_x = (int)(wall->wall_x * tex->w);
	y = wall->draw_start;
	while (y <= wall->draw_end)
	{
		tex_y = ((y - wall->draw_start) * tex->h) / wall->line_height;
		color = *(unsigned int *)(tex->addr + (tex_y * tex->line_len)
				+ ((int)wall->tex_x * (tex->bpp / 8)));
		my_mlx_pixel_put(&game->screen, x, y, color);
		y++;
	}
}

void	render_walls(t_game *game)
{
	int		x;
	double	camera_x;
	t_ray	ray;
	t_wall	wall;

	x = 0;
	while (x < game->screen.w)
	{
		camera_x = 2 * ((double)x / game->screen.w) - 1;
		ray.ray_dir_x = game->player.dir_x + camera_x * game->player.plane_x;
		ray.ray_dir_y = game->player.dir_y + camera_x * game->player.plane_y;
		ray.map_x = (int)game->player.x;
		ray.map_y = (int)game->player.y;
		init_dda(&game->player, &ray);
		dda_loop(&ray, game->map.grid);
		calc_wall_height(&ray, &wall, game->screen.h);
		paint_wall_column(game, x, &ray, &wall);
		x++;
	}
}
