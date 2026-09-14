#include "cub3d.h"

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
		dda_loop(&ray, game);
		calc_wall_height(&ray, &wall, game->screen.h);
		paint_wall_column(game, x, &ray, &wall);
		x++;
	}
}

int	game_loop(t_game *game)
{
	process_movement(game);
	paint_ceiling_floor(game);
	render_walls(game);
	mlx_put_image_to_window(game->mlx, game->win, game->screen.ptr, 0, 0);
	return (0);
}

void	load_one_texture(t_game *game, char *path, t_img *tex_img)
{
	int	width;
	int	height;

	tex_img->ptr = mlx_xpm_file_to_image(game->mlx, path, &width, &height);
	if (!tex_img->ptr)
		error_exit_game(game, "Failed to load texture.");
	tex_img->addr = mlx_get_data_addr(tex_img->ptr, &tex_img->bpp,
			&tex_img->line_len, &tex_img->endian);
	if (!tex_img->addr)
		error_exit_game(game, "Failed to get texture address.");
	tex_img->w = width;
	tex_img->h = height;
}

void	load_all_textures(t_game *game)
{
	load_one_texture(game, game->tex.no_path, &game->tex.no_img);
	load_one_texture(game, game->tex.so_path, &game->tex.so_img);
	load_one_texture(game, game->tex.we_path, &game->tex.we_img);
	load_one_texture(game, game->tex.ea_path, &game->tex.ea_img);
}
