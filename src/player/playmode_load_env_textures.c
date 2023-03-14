/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_env_textures.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:07:37 by raho              #+#    #+#             */
/*   Updated: 2023/03/14 13:34:18 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "tga.h"

static void	unpack_and_save_texture(t_sdlcontext *sdl, char *env_texture)
{
	static int	i = 0;

	load_and_write_filecontent(LEVEL0FILE, env_texture, TEMPIMGENV);
	sdl->env_textures[i] = tgaparse(TEMPIMGENV);
	if (sdl->env_textures[i].data != NULL)
		ft_strcpy(sdl->env_textures[i].name, extract_filename(env_texture));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and saved .tga file:", sdl->env_textures[i].name, NULL});
	i++;
	remove(TEMPIMGENV);
}

static int	parse_image_env_list(int fd, t_sdlcontext *sdl)
{
	int		ret;
	char	*env_texture;

	env_texture = NULL;
	ret = get_next_line(fd, &env_texture);
	while (ret)
	{
		if (env_texture)
		{
			unpack_and_save_texture(sdl, env_texture);
			free(env_texture);
			env_texture = NULL;
		}
		ret = get_next_line(fd, &env_texture);
	}
	return (ret);
}

void	playmode_load_env_textures(t_sdlcontext *sdl)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING ENV_TEXTURES");
	load_and_write_filecontent(LEVEL0FILE, IMGENVLISTPATH, TEMPIMGENVLIST);
	sdl->env_texturecount = count_asset_list(TEMPIMGENVLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPIMGENVLIST, "size =", s_itoa(sdl->env_texturecount), NULL});
	sdl->env_textures = ft_memalloc(sizeof(t_img) * sdl->env_texturecount);
	if (sdl->env_textures == NULL)
		doomlog(LOG_EC_MALLOC, "playmode_load_env_textures");
	fd = fileopen(TEMPIMGENVLIST, O_RDONLY);
	ret = parse_image_env_list(fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_env_textures");
	fileclose(fd, TEMPIMGENVLIST);
	remove(TEMPIMGENVLIST);
}
