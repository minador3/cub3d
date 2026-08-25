/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:30 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:21:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	count_commas(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			count++;
		i++;
	}
	return (count);
}

static int	parse_rgb_value(char **str)
{
	int		val;
	int		count;
	char	*s;

	s = *str;
	s = skip_spaces(s);
	if (!ft_isdigit(*s))
		return (-1);
	val = 0;
	count = 0;
	while (ft_isdigit(*s))
	{
		val = val * 10 + (*s - '0');
		count++;
		if (count > 3 || val > 255)
			return (-1);
		s++;
	}
	s = skip_spaces(s);
	*str = s;
	return (val);
}

static int	parse_color(char *line, int color_arr[3])
{
	char	*s;
	int		i;

	s = line + 1;
	s = skip_spaces(s);
	if (count_commas(s) != 2)
		return (0);
	i = 0;
	while (i < 3)
	{
		color_arr[i] = parse_rgb_value(&s);
		if (color_arr[i] < 0 || color_arr[i] > 255)
			return (0);
		if (i < 2)
		{
			if (*s != ',')
				return (0);
			s++;
		}
		i++;
	}
	if (*s != '\0' && *s != '\n' && *s != '\r')
		return (0);
	return (1);
}

static int	parse_texture(char *line, char **path_dst)
{
	char	*s;
	int		len;

	if (*path_dst)
		return (0);
	s = line + 2;
	s = skip_spaces(s);
	if (*s == '\0' || *s == '\n' || *s == '\r')
		return (0);
	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		len--;
	if (len > 0 && s[len - 1] == '\r')
		len--;
	while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t'))
		len--;
	*path_dst = ft_substr(s, 0, len);
	if (!*path_dst)
		return (0);
	return (1);
}

int	parse_config_line(t_game *game, char *line)
{
	char	*trimmed;

	trimmed = skip_spaces(line);
	if (*trimmed == '\0' || *trimmed == '\n' || *trimmed == '\r')
		return (0);
	if (ft_strncmp(trimmed, "NO ", 3) == 0 || ft_strncmp(trimmed, "NO\t", 3) == 0)
	{
		if (!parse_texture(trimmed, &game->tex.no_path))
			return (-1);
		return (1);
	}
	else if (ft_strncmp(trimmed, "SO ", 3) == 0 || ft_strncmp(trimmed, "SO\t", 3) == 0)
	{
		if (!parse_texture(trimmed, &game->tex.so_path))
			return (-1);
		return (1);
	}
	else if (ft_strncmp(trimmed, "WE ", 3) == 0 || ft_strncmp(trimmed, "WE\t", 3) == 0)
	{
		if (!parse_texture(trimmed, &game->tex.we_path))
			return (-1);
		return (1);
	}
	else if (ft_strncmp(trimmed, "EA ", 3) == 0 || ft_strncmp(trimmed, "EA\t", 3) == 0)
	{
		if (!parse_texture(trimmed, &game->tex.ea_path))
			return (-1);
		return (1);
	}
	else if (ft_strncmp(trimmed, "F ", 2) == 0 || ft_strncmp(trimmed, "F\t", 2) == 0)
	{
		if (game->map.has_floor)
			return (-1);
		if (!parse_color(trimmed, game->map.floor_color))
			return (-1);
		game->map.has_floor = 1;
		game->map.floor_hex = (game->map.floor_color[0] << 16)
			| (game->map.floor_color[1] << 8) | game->map.floor_color[2];
		return (1);
	}
	else if (ft_strncmp(trimmed, "C ", 2) == 0 || ft_strncmp(trimmed, "C\t", 2) == 0)
	{
		if (game->map.has_ceil)
			return (-1);
		if (!parse_color(trimmed, game->map.ceil_color))
			return (-1);
		game->map.has_ceil = 1;
		game->map.ceil_hex = (game->map.ceil_color[0] << 16)
			| (game->map.ceil_color[1] << 8) | game->map.ceil_color[2];
		return (1);
	}
	return (-1);
}

int	is_config_complete(t_game *game)
{
	return (game->tex.no_path != NULL
		&& game->tex.so_path != NULL
		&& game->tex.we_path != NULL
		&& game->tex.ea_path != NULL
		&& game->map.has_floor
		&& game->map.has_ceil);
}
