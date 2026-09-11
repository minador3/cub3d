#include "cub3d.h"

void	move_forward(t_game *game, double speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.dir_x * speed;
	new_y = game->player.y + game->player.dir_y * speed;
	if (game->map.grid[(int)game->player.y][(int)new_x] != '1')
		game->player.x = new_x;
	if (game->map.grid[(int)new_y][(int)game->player.x] != '1')
		game->player.y = new_y;
}

void	move_backward(t_game *game, double speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.dir_x * speed;
	new_y = game->player.y - game->player.dir_y * speed;
	if (game->map.grid[(int)game->player.y][(int)new_x] != '1')
		game->player.x = new_x;
	if (game->map.grid[(int)new_y][(int)game->player.x] != '1')
		game->player.y = new_y;
}

void	strafe_right(t_game *game, double speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.plane_x * speed;
	new_y = game->player.y + game->player.plane_y * speed;
	if (game->map.grid[(int)game->player.y][(int)new_x] != '1')
		game->player.x = new_x;
	if (game->map.grid[(int)new_y][(int)game->player.x] != '1')
		game->player.y = new_y;
}

void	strafe_left(t_game *game, double speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.plane_x * speed;
	new_y = game->player.y - game->player.plane_y * speed;
	if (game->map.grid[(int)game->player.y][(int)new_x] != '1')
		game->player.x = new_x;
	if (game->map.grid[(int)new_y][(int)game->player.x] != '1')
		game->player.y = new_y;
}

void	rotate_player(t_game *game, double theta)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	game->player.dir_x = old_dir_x
		* cos(theta) - game->player.dir_y * sin(theta);
	game->player.dir_y = old_dir_x
		* sin(theta) + game->player.dir_y * cos(theta);
	old_plane_x = game->player.plane_x;
	game->player.plane_x = old_plane_x
		* cos(theta) - game->player.plane_y * sin(theta);
	game->player.plane_y = old_plane_x
		* sin(theta) + game->player.plane_y * cos(theta);
}
