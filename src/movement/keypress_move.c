#include "cub3d.h"

void	strafe_left(t_game *game, double speed)
{
	strafe_right(game, -speed);
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

int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == 65307 || keycode == 53)
		close_game(game);
	else if (keycode == 119 || keycode == 87 || keycode == 13)
		game->keys.w = 1;
	else if (keycode == 115 || keycode == 83 || keycode == 1)
		game->keys.s = 1;
	else if (keycode == 97 || keycode == 65 || keycode == 0)
		game->keys.a = 1;
	else if (keycode == 100 || keycode == 68 || keycode == 2)
		game->keys.d = 1;
	else if (keycode == 65361 || keycode == 123)
		game->keys.left = 1;
	else if (keycode == 65363 || keycode == 124)
		game->keys.right = 1;
	return (0);
}

int	handle_keyrelease(int keycode, t_game *game)
{
	if (keycode == 119 || keycode == 87 || keycode == 13)
		game->keys.w = 0;
	else if (keycode == 115 || keycode == 83 || keycode == 1)
		game->keys.s = 0;
	else if (keycode == 97 || keycode == 65 || keycode == 0)
		game->keys.a = 0;
	else if (keycode == 100 || keycode == 68 || keycode == 2)
		game->keys.d = 0;
	else if (keycode == 65361 || keycode == 123)
		game->keys.left = 0;
	else if (keycode == 65363 || keycode == 124)
		game->keys.right = 0;
	return (0);
}

void	process_movement(t_game *game)
{
	if (game->keys.w)
		move_forward(game, MOVE_SPEED);
	if (game->keys.s)
		move_backward(game, MOVE_SPEED);
	if (game->keys.a)
		strafe_left(game, MOVE_SPEED);
	if (game->keys.d)
		strafe_right(game, MOVE_SPEED);
	if (game->keys.left)
		rotate_player(game, -ROTATE_SPEED);
	if (game->keys.right)
		rotate_player(game, ROTATE_SPEED);
}
