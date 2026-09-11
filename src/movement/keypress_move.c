#include "cub3d.h"

int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == 65307)
		close_game(game);
	else if (keycode == 119)
		game->keys.w = 1;
	else if (keycode == 115)
		game->keys.s = 1;
	else if (keycode == 97)
		game->keys.a = 1;
	else if (keycode == 100)
		game->keys.d = 1;
	else if (keycode == 65361)
		game->keys.left = 1;
	else if (keycode == 65363)
		game->keys.right = 1;
	return (0);
}

int	handle_keyrelease(int keycode, t_game *game)
{
	if (keycode == 119)
		game->keys.w = 0;
	else if (keycode == 115)
		game->keys.s = 0;
	else if (keycode == 97)
		game->keys.a = 0;
	else if (keycode == 100)
		game->keys.d = 0;
	else if (keycode == 65361)
		game->keys.left = 0;
	else if (keycode == 65363)
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
