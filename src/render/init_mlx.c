#include "cub3d.h"

void	init_window(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		error_exit_game(game, "Failed to initialize MLX.");
	game->win = mlx_new_window(game->mlx, 800, 600, "cub3D");
	if (!game->win)
		error_exit_game(game, "Failed to create window.");
	game->screen.ptr = mlx_new_image(game->mlx, 800, 600);
	if (!game->screen.ptr)
		error_exit_game(game, "Failed to create screen image.");
	game->screen.addr = mlx_get_data_addr(game->screen.ptr, &game->screen.bpp,
			&game->screen.line_len, &game->screen.endian);
	if (!game->screen.addr)
		error_exit_game(game, "Failed to get screen address.");
	game->screen.w = 800;
	game->screen.h = 600;
}
