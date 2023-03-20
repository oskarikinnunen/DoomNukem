/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_env_textures.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:07:37 by raho              #+#    #+#             */
/*   Updated: 2023/03/20 17:21:59 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "tga.h"

static void	unpack_and_load_texture(int txtr_i, char *level_path,
									char *env_texture, t_sdlcontext *sdl)
{
	load_and_write_filecontent(level_path, env_texture, TEMPIMGENV);
	sdl->env_textures[txtr_i] = tgaparse(TEMPIMGENV);
	if (sdl->env_textures[txtr_i].data != NULL)
		ft_strcpy(sdl->env_textures[txtr_i].name, \
					extract_filename(env_texture));
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .tga file:", \
			sdl->env_textures[txtr_i].name, NULL});
	remove(TEMPIMGENV);
}

static int	parse_image_env_list(int fd, char *level_path, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*env_texture;

	i = 0;
	env_texture = NULL;
	ret = get_next_line(fd, &env_texture);
	while (ret)
	{
		if (env_texture)
		{
			unpack_and_load_texture(i, level_path, env_texture, sdl);
			free(env_texture);
			env_texture = NULL;
			i++;
		}
		ret = get_next_line(fd, &env_texture);
	}
	return (ret);
}

void	playmode_load_env_textures(char *level_path, t_sdlcontext *sdl)
{
	int	fd;
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING ENV_TEXTURES");
	load_and_write_filecontent(level_path, IMGENVLISTPATH, TEMPIMGENVLIST);
	sdl->env_texturecount = count_asset_list(TEMPIMGENVLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPIMGENVLIST, "size =", s_itoa(sdl->env_texturecount), NULL});
	sdl->env_textures = prot_memalloc(sizeof(t_img) * sdl->env_texturecount);
	fd = fileopen(TEMPIMGENVLIST, O_RDONLY);
	ret = parse_image_env_list(fd, level_path, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_env_textures");
	fileclose(fd, TEMPIMGENVLIST);
	remove(TEMPIMGENVLIST);
}
