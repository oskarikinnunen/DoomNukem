/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_tool_paint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:52:25 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/24 14:36:53 by raho             ###   ########.fr       */
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
	while (i < sdl->env_texturecount)
	{
		if (&sdl->env_textures[i] == img)
			return (i);
		i++;
	}
	return (prev);
}

t_area	*get_floor_room(t_world *world, t_entity *entity)
{
	t_list	*l;
	t_area	*r;
	int		i;

	l = world->arealist;
	r = NULL;
	while (l != NULL)
	{
		r = l->content;
		i = 0;
		while (i < r->floorcount)
		{
			if (entity == r->floors[i].entity)
				return (r);
			i++;
		}
		l = l->next;
	}
	return (NULL);
}

void	apply_floortexture(t_world *world, t_area *room, char *texname) //TODO: remove
{
	int			i;
	t_meshtri	*floor;

	i = 0;
	ft_strncpy_term(room->s_floortex.str, texname, 60);
}

static t_wall	*_room_get_wall_with_entity(t_area *room, t_entity *entitymatch)
{
	int	i;

	i = 0;
	while (i < room->wallcount)
	{
		if (room->walls[i].entity == entitymatch)
			return (&room->walls[i]);
		i++;
	}
	return (NULL);
}

static t_area	*_world_get_room_with_entity(t_world *world, t_entity *entitymatch)
{
	t_list	*l;
	t_area	*room;

	l = world->arealist;
	while (l != NULL)
	{
		room = l->content;
		if (_room_get_wall_with_entity(room, entitymatch) != NULL)
			return (room);
		l = l->next;
	}
	return (NULL);
}

static t_wall	*_world_get_wall_with_entity(t_world *world, t_entity *entitymatch)
{
	t_list	*l;
	t_area	*room;
	t_wall	*found;

	l = world->arealist;
	while (l != NULL)
	{
		room = l->content;
		found = _room_get_wall_with_entity(room, entitymatch);
		if (found != NULL)
			return (found);
		l = l->next;
	}
	return (NULL);
}

static t_wall	*_world_get_floor_with_entity(t_world *world, t_entity *entitymatch)
{
	t_list	*l;
	t_area	*room;
	t_wall	*found;

	l = world->arealist;
	while (l != NULL)
	{
		room = l->content;
		found = _room_get_wall_with_entity(room, entitymatch);
		if (found != NULL)
			return (found);
		l = l->next;
	}
	return (NULL);
}

static void paint_room(t_entity *hit_ent, t_img *tex, t_world *world)
{
	t_list	*l;
	int		i;

	l = world->arealist;
	while (l != NULL)
	{
		t_area *area = l->content;
		i = 0;
		while (i < area->wallcount)
		{
			if (area->walls[i].entity == hit_ent)
			{
				ft_strncpy_term(area->walls[i].s_walltex.str, tex->name, 60);
				room_init(area, world);
				return;
			}
			i++;
		}
		i = 0;
		while (i < area->floorcount)
		{
			if (area->floors[i].entity == hit_ent)
			{
				ft_strncpy_term(area->s_floortex.str, tex->name, 60);
				room_init(area, world);
				return;
			}
			i++;
		}
		i = 0;
		while (i < area->ceilingcount)
		{
			if (area->ceilings[i].entity == hit_ent)
			{
				ft_strncpy_term(area->s_ceiltex.str, tex->name, 60);
				room_init(area, world);
				return;
			}
			i++;
		}
		l = l->next;
	}
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
		&& ent != NULL && ent->rigid && ent->obj != NULL)
		img_index = get_image_index(sdl, ent->obj->materials->img, img_index);
	gui_end(gui);
	int	delt;
	delt = 0;
	if (ed->hid.mouse.scroll_delta > 0)
		delt = 1;
	else if (ed->hid.mouse.scroll_delta < 0)
		delt = -1;
	img_index += delt;
	img_index = ft_clamp(img_index, 0, sdl->env_texturecount - 1);
	tex = &sdl->env_textures[img_index];
	
	if (img_index != prev_img_index)
	{
		prev_changetime = ed->world.clock.time;
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
	if (ed->world.clock.time < prev_changetime + 1000)
	{
		i = -5;
		while (i < 6)
		{

			cur = point_add(middle, point_mul((t_point){36, 0}, i));
			cur.y = middle.y + (cos(i / 5.0f) * 164);
			if (img_index + i >= 0 && img_index + i < sdl->env_texturecount)
			{
				draw_image(*sdl, cur, sdl->env_textures[img_index + i], (t_point){32,32});
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
		if (img_index + i >= 0 && img_index + i < sdl->env_texturecount)
		{
			draw_image(*sdl, cur, sdl->env_textures[img_index + i], (t_point){32,32});
			if (i == 0)
				draw_rectangle(*sdl, rect, AMBER_4);
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
		paint_room(ent, tex, &ed->world);
		//create_lightmap_for_entity(ent, &ed->world);
		//create_map_for_entity(ent, &ed->world);
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
		dat->rtm = rtm_none;
}