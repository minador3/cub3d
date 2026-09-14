#include "cub3d.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int colour)
{
	int	offset;

	if (x < 0 || x >= img->w || y < 0 || y >= img->h)
		return ;
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
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (&tex->ea_img);
		return (&tex->we_img);
	}
	else
	{
		if (ray->step_y > 0)
			return (&tex->so_img);
		return (&tex->no_img);
	}
}

static void	draw_slice(t_game *game, int x, t_wall *wall, t_img *tex)
{
	int		y;
	int		tex_y;
	double	step;
	double	tex_pos;

	step = 1.0 * tex->h / wall->line_height;
	tex_pos = (wall->draw_start - game->screen.h / 2.0
			+ wall->line_height / 2.0) * step;
	y = wall->draw_start;
	while (y <= wall->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->h)
			tex_y = tex->h - 1;
		tex_pos += step;
		my_mlx_pixel_put(&game->screen, x, y,
			*(unsigned int *)(tex->addr + (tex_y * tex->line_len)
				+ (wall->tex_x * (tex->bpp / 8))));
		y++;
	}
}

void	paint_wall_column(t_game *game, int x, t_ray *ray, t_wall *wall)
{
	t_img	*tex;

	if (ray->side == 0)
		wall->wall_x = game->player.y + wall->perp_wall_dist * ray->ray_dir_y;
	else
		wall->wall_x = game->player.x + wall->perp_wall_dist * ray->ray_dir_x;
	wall->wall_x -= floor(wall->wall_x);
	tex = select_texture(ray, &game->tex);
	wall->tex_x = (int)(wall->wall_x * tex->w);
	if ((ray->side == 0 && ray->ray_dir_x < 0)
		|| (ray->side == 1 && ray->ray_dir_y > 0))
		wall->tex_x = tex->w - wall->tex_x - 1;
	if (wall->tex_x < 0)
		wall->tex_x = 0;
	if (wall->tex_x >= tex->w)
		wall->tex_x = tex->w - 1;
	if (wall->line_height <= 0)
		wall->line_height = 1;
	draw_slice(game, x, wall, tex);
}
