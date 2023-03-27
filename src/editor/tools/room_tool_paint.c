/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_paint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:52:25 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 19:44:57 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

int	get_image_index(t_sdlcontext *sdl, t_img *img, int prev)
{
	int	i;

	i = 0;
	while (i < sdl->env_texturecount)
	{
		if (&sdl->env_textures[i] == img)
			return (i);
		i++;
	}
	return (prev);
}

static void	draw_scrollgui(t_editor *ed, t_sdlcontext *sdl,
		int img_index, t_img *tex)
{
	t_point	middle;
	t_point	cur;
	int		i;

	middle = point_div(sdl->screensize, 2);
	middle = point_sub(middle, (t_point){16, 16});
	middle = point_add(middle, (t_point){0, 32});
	i = -5;
	while (i < 6)
	{
		cur = point_add(middle, point_mul((t_point){36, 0}, i));
		cur.y = middle.y + (cos(i / 5.0f) * 164);
		if (img_index + i >= 0 && img_index + i < sdl->env_texturecount)
			draw_image(*sdl, cur, sdl->env_textures[img_index + i],
				(t_point){32, 32});
		i++;
	}
	print_text_boxed(sdl, tex->name, point_add(cur, (t_point){0, 40}));
}

int	normalized_scroll(int scroll)
{
	int	n;

	n = 0;
	if (scroll > 0)
		n = 1;
	else if (scroll < 0)
		n = -1;
	return (n);
}

//Not to be confused with paint_can
bool	can_paint(t_editor *ed, t_entity *ent, t_img *tex)
{
	return ((ed->hid.mouse.held == MOUSE_LEFT
			&& ent != NULL
			&& ent->rigid
			&& ent->obj->materials->img != tex && tex != NULL
			&& ent->obj != NULL));
}

void	room_tool_paint(t_editor *ed, t_sdlcontext *sdl, t_room_tool_data *dat)
{
	t_autogui		*gui;
	static t_img	*tex;
	static int		img_index;
	int				i;
	t_entity		*ent;

	ent = dat->raycast_info.hit_entity;
	gui = &dat->paint_gui;
	gui_start(gui);
	if (gui_shortcut_button("Copy texture", 'C', gui)
		&& ent != NULL && ent->rigid && ent->obj != NULL)
		img_index = get_image_index(sdl, ent->obj->materials->img, img_index);
	gui_end(gui);
	img_index += normalized_scroll(ed->hid.mouse.scroll_delta);
	img_index = ft_clamp(img_index, 0, sdl->env_texturecount - 1);
	tex = &sdl->env_textures[img_index];
	draw_scrollgui(ed, sdl, img_index, tex);
	if (can_paint(ed, ent, tex))
		paint_room(ent, tex, &ed->world);
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->rtm = rtm_none;
}
