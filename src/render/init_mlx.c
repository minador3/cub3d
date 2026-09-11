#include "cub3d.h"

void	init_window(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		error_exit("Failed to initialize MLX.");
	game->win = mlx_new_window(game->mlx, 800, 600, "cub3D");
	if (!game->win)
		error_exit("Failed to create window.");
	game->screen.ptr = mlx_new_image(game->mlx, 800, 600);
	game->screen.addr = mlx_get_data_addr(game->screen.ptr, &game->screen.bpp,
			&game->screen.line_len, &game->screen.endian);
	game->screen.w = 800;
	game->screen.h = 600;
}
