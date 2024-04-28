/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playmode_load_env_textures.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:07:37 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:04:49 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "file_io.h"
#include "assets.h"
#include "tga.h"

static void	unpack_and_load_texture(int txtr_i, int level_fd,
									char *env_texture, t_sdlcontext *sdl)
{
	load_and_write_filecontent(level_fd, env_texture, TEMPIMGENV);
	sdl->env_textures[txtr_i] = tga_parse(TEMPIMGENV);
	if (sdl->env_textures[txtr_i].data != NULL)
		ft_strncpy_term(sdl->env_textures[txtr_i].name, \
					extract_file_name(env_texture), 120);
	doomlog_mul(LOG_NORMAL, (char *[3]){\
			"unpacked and loaded .tga file:", \
			sdl->env_textures[txtr_i].name, NULL});
}

static int	parse_image_env_list(int level_fd, t_sdlcontext *sdl)
{
	int		ret;
	int		i;
	char	*env_texture;
	int		temp_fd;

	i = 0;
	env_texture = NULL;
	temp_fd = ft_fileopen(TEMPIMGENVLIST, O_RDONLY);
	ret = get_next_line(temp_fd, &env_texture);
	while (ret)
	{
		if (env_texture)
		{
			unpack_and_load_texture(i, level_fd, env_texture, sdl);
			free(env_texture);
			env_texture = NULL;
			i++;
		}
		ret = get_next_line(temp_fd, &env_texture);
	}
	ft_fileclose(temp_fd, TEMPIMGENVLIST);
	return (ret);
}

void	playmode_load_env_textures(int level_fd, t_sdlcontext *sdl)
{
	int	ret;

	doomlog(LOG_NORMAL, "UNPACKING ENV_TEXTURES");
	load_and_write_filecontent(level_fd, IMGENVLISTPATH, TEMPIMGENVLIST);
	sdl->env_texturecount = count_asset_list(TEMPIMGENVLIST);
	doomlog_mul(LOG_NORMAL, (char *[4]){\
			TEMPIMGENVLIST, "size =", s_itoa(sdl->env_texturecount), NULL});
	sdl->env_textures = prot_memalloc(sizeof(t_img) * sdl->env_texturecount);
	ret = parse_image_env_list(level_fd, sdl);
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "playmode_load_env_textures");
}
