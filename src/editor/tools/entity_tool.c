/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/04 20:40:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "inputhelp.h"
#include "objects.h"
/*
void entity_tool_append_list(t_editor *ed, t_entity ent) //TODO: make 
{
	list_push(&ed->entitylist, &ent, sizeof(t_entity));
}

static t_entity	*find_closest(t_editor *ed) //TODO: pointer
{
	t_list		*l;
	t_entity	*cur_ent;
	t_point		entity_pos;

	l = ed->entitylist;
	while (l != NULL)
	{
		cur_ent = (t_entity *)l->content;
		entity_pos = vector2_to_point(cur_ent->position);
		if (point_sqr_magnitude(point_sub(mousetoworldspace(ed), entity_pos)) < 400)
		{
			return (cur_ent);
		}
		l = l->next;
	}
	return (NULL);
}

static void entity_tool_place(t_editor *ed)
{
	t_entity		*ent;

	ent = (t_entity *)ed->tool->tooldata;
	ent->position = point_to_vector2(mousetoworldspace(ed));
	entity_tool_append_list(ed, *ent);
	ed->mouse.click_unhandled = false;
}

static void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	static t_img	*entity_img;
	t_entity		*ent;
	t_list			*l;

	if (entity_img == NULL)
		entity_img = get_image_by_name(sdl, "entitytool.png");
	l = ed->entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		draw_image(sdl, worldtoeditorspace(ed, ent->position), *entity_img, entity_img->size);
		l = l->next;
	}
	draw_image(sdl, ed->mouse.pos, *entity_img, entity_img->size);
	ent = find_closest(ed);
	if (ent != NULL)
		drawcircle(sdl, worldtoeditorspace(ed, ent->position), 20, CLR_GREEN);
	return ;
}

static void	entity_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	t_entity *hover;

	hover = find_closest(ed);
	if (ed->mouse.held == MOUSE_LEFT && hover != NULL)
		hover->position = point_to_vector2(mousetoworldspace(ed));
	if (ed->mouse.click_unhandled)
	{
		if (hover == NULL && ed->mouse.click_button == MOUSE_LEFT)
			entity_tool_place(ed);
		else if (ed->mouse.click_button == MOUSE_RIGHT)
			list_remove(&ed->entitylist, hover, sizeof(t_entity));
		ed->mouse.click_unhandled = false;
	}
		
}

static void	entity_tool_cleanup(t_editor *ed)
{
	SDL_ShowCursor(SDL_ENABLE);
}
*/

/*void	entity_tool_update(t_editor *ed)
{

}*/

char	*floatstr(float f)
{
	static char	final[8];
	char	*strinteger;
	char	*strdecimal;
	int		neg;
	int		i;

	i = (int)f;
	f = f - i;
	int dec = 1;
	f = f * (ft_pow(10, dec));
	neg = (f < 0.0f);
	strinteger = ft_itoa(ft_abs(i));
	strdecimal = ft_itoa(ft_abs((int)f));
	//final = ft_strnew(ft_strlen(strinteger) + ft_strlen(strdecimal) + 1 + neg);
	ft_bzero(final, sizeof(final));
	if (neg)
		ft_strcat(final, "-");
	ft_strcat(final, strinteger);
	ft_strcat(final, ",");
	ft_strcat(final, strdecimal);
	free(strinteger);
	free(strdecimal);
	return (final);
}

static t_img	black_image()
{
	static t_img	black;

	black.data = (uint32_t[1]) {0};
	black.length = 1;
	black.size = point_one();
	return (black);
}

static void	draw_transform_info(t_transform t, t_sdlcontext sdl)
{
	draw_image(sdl, (t_point){17, 100}, black_image(), (t_point){180, 58});
	draw_text_boxed(&sdl, "POS  :", (t_point){20, 143}, (t_point){sdl.window_w, sdl.window_h});
	draw_text_boxed(&sdl, vector_string(t.location), (t_point){65, 143}, (t_point){sdl.window_w, sdl.window_h});
	draw_text_boxed(&sdl, "SCALE:", (t_point){20, 105}, (t_point){sdl.window_w, sdl.window_h});
	draw_text_boxed(&sdl, vector_string(t.scale), (t_point){65, 105}, (t_point){sdl.window_w, sdl.window_h});
}

static void	draw_current_operation(t_entity *ent, t_entity *collide, t_sdlcontext sdl)
{
	if (collide == NULL)
	{
		draw_text_boxed(&sdl, "ADD:", (t_point){sdl.window_w / 2, sdl.window_h - 25}, (t_point){sdl.window_w, sdl.window_h});
		draw_text_boxed(&sdl, ent->obj->name, (t_point){sdl.window_w / 2 + 60, sdl.window_h - 25}, (t_point){sdl.window_w, sdl.window_h});
	}
	else
	{
		draw_text_boxed(&sdl, "DEL:", (t_point){sdl.window_w / 2, sdl.window_h - 25}, (t_point){sdl.window_w, sdl.window_h});
		draw_text_boxed(&sdl, collide->obj->name, (t_point){sdl.window_w / 2 + 60, sdl.window_h - 25}, (t_point){sdl.window_w, sdl.window_h});
	}
}

void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_entity	*ent;
	t_entity	*collide;

	ent = (t_entity *)ed->tool->tooldata;
	ent->object_index = ft_clamp(ent->object_index, 0, sdl.objectcount - 1);
	if (ent->obj != &sdl.objects[ent->object_index])
		ent->obj = &sdl.objects[ent->object_index];
	if (ent->obj == NULL)
		ent->obj = object_plane();
	/* SPLIT HERE */
	//ed->render.wireframe = true;
	ed->render.gizmocolor = INT_MAX;
	collide = entity_collides(ed->world.physics, *ent);
	if (collide != NULL)
	{
		ed->render.gizmocolor = CLR_PRPL;
		render_object(sdl, ed->render, collide);
	}
	render_object(sdl, ed->render, ent);
	
	//ed->render.wireframe = false;
	draw_colliders(ed->world.physics, sdl, ed->render);
	/* END SPLIT */
	set_font_size(&sdl, 0);
	draw_transform_info(ent->transform, sdl);
	draw_current_operation(ent, collide, sdl);
	if (instantbutton((t_rectangle) {30, 120, 20, 20}, &ed->mouse, sdl, "minus.png"))
		ent->transform.scale = vector3_add_xyz(ent->transform.scale, -0.25f);
	if (instantbutton((t_rectangle) {52, 120, 20, 20}, &ed->mouse, sdl, "one.png"))
		ent->transform.scale = vector3_one();
	if (instantbutton((t_rectangle) {74, 120, 20, 20}, &ed->mouse, sdl, "plus.png"))
		ent->transform.scale = vector3_add_xyz(ent->transform.scale, 0.25f);
}

void	entity_tool_update(t_editor *ed)
{
	t_entity	*ent;
	t_vector3	dir;

	sizeof(t_sdlcontext);
	ent = (t_entity *)ed->tool->tooldata;
	dir = vector3_sub((t_vector3){ed->position.x, ed->position.y, 20.0f}, ent->transform.location);
	ent->transform.location = vector3_movetowards(ent->transform.location, dir, ed->clock.delta * 1.0f);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		ent->object_index += ed->mouse.scroll_delta;

	if (mouse_clicked(ed->mouse, MOUSE_LEFT))
	{
		t_entity *collision_ent = entity_collides(ed->world.physics, *ent);
		if (collision_ent == NULL)
		{
			list_push(&ed->world.entitylist, ent, sizeof(t_entity));
			calculate_colliders_for_entities(&ed->world);
		}
	}
	if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
	{
		t_entity *collision_ent = entity_collides(ed->world.physics, *ent);
		if (collision_ent != NULL)
		{
			list_remove(&ed->world.entitylist, collision_ent, sizeof(t_entity));
			calculate_colliders_for_entities(&ed->world);
		}
	}
}

t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		entity_tool_update, entity_tool_draw 
	};
	t_entity		*ent; //TODO: make entity tool use it's own tooldata struct,

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_entity));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		ent = (t_entity *)tool.tooldata;
		ent->transform.scale = vector3_one();
		ent->object_index = 1;
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
