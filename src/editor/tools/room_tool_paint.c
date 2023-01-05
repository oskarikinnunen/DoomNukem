/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_paint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:52:25 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/04 18:26:49 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools/roomtool.h"
#include "editor_tools.h"
#include "doomnukem.h"
#include "objects.h"

int		get_image_index(t_sdlcontext *sdl, t_img *img, int prev)
{
	int	i;

	i = 0;
	while (i < sdl->texturecount)
	{
		if (&sdl->textures[i] == img)
			return (i);
		i++;
	}
	return (prev);
}

void	room_tool_paint(t_editor *ed, t_sdlcontext *sdl, t_roomtooldata *dat)
{
	t_autogui		*gui;
	static	t_img	*tex;
	static			int	img_index;
	static			int	prev_img_index;
	static			uint32_t	prev_changetime;
	int				i;
	t_entity		*ent;

	ent = dat->raycastinfo.hit_entity;

	gui = &dat->paint_gui;
	gui_start(gui);
	if (gui_shortcut_button("Copy texture", 'C', gui)
		&& ent != NULL && ent->rigid)
	{
		img_index = get_image_index(sdl, ent->obj->materials->img, img_index);
	}

	gui_end(gui);
	int	delt;
	delt = 0;
	if (ed->hid.mouse.scroll_delta > 0)
		delt = 1;
	else if (ed->hid.mouse.scroll_delta < 0)
		delt = -1;
	img_index += delt;
	img_index = ft_clamp(img_index, 0, sdl->texturecount - 1);
	tex = &sdl->textures[img_index];
	
	if (img_index != prev_img_index)
	{
		prev_changetime = ed->world.clock.prev_time;
	}
	t_point middle = point_div(sdl->screensize, 2);
	middle = point_sub(middle, (t_point){16,16});
	middle = point_add(middle, (t_point){0,32});
	
	t_rectangle mrect;
	mrect.position = middle;
	mrect.size = (t_point){32,32};
	//draw_image(*sdl, point_div(sdl->screensize, 2), *tex, (t_point){64,64});

	int from = 0;
	int	to = 1;
	t_point cur;
	if (ed->clock.prev_time < prev_changetime + 1000)
	{
		i = -5;
		while (i < 6)
		{

			cur = point_add(middle, point_mul((t_point){36, 0}, i));
			cur.y = middle.y + (cos(i / 5.0f) * 164);
			if (img_index + i >= 0 && img_index + i < sdl->texturecount)
			{
				draw_image(*sdl, cur, sdl->textures[img_index + i], (t_point){32,32});
			}
			i++;
		}
	}
	i = from;
	while (i < to)
	{

		cur = point_add(middle, point_mul((t_point){32, 0}, i));
		cur.y = middle.y + (cos(i / 5.0f) * 164);
		t_rectangle rect;
		rect.position = cur;
		rect.size = (t_point){32,32};
		if (img_index + i >= 0 && img_index + i < sdl->texturecount)
		{
			draw_image(*sdl, cur, sdl->textures[img_index + i], (t_point){32,32});
			if (i == 0)
				drawrectangle(*sdl, rect, AMBER_4);
		}
		i++;
	}
	print_text_boxed(sdl, tex->name, point_add(cur, (t_point){0, 40}));
	//drawrectangle(*sdl, mrect, AMBER_4);
	prev_img_index = img_index;
	if (ed->hid.mouse.held == MOUSE_LEFT &&
		ent != NULL &&
		ent->rigid &&
		ent->obj->materials->img != tex && tex != NULL
		&& ent->obj != NULL)
	{
		printf("applying tex \n");
		ent->obj->materials->img = tex;
		create_lightmap_for_entity(ent, &ed->world);
		create_map_for_entity(ent, &ed->world);
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->rtm = rtm_none;
}