/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 15:29:16 by vlaine           ###   ########.fr       */
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
	sdl.font.color = sdl.font.font_colors.white;
	draw_image(sdl, (t_point){17, 100}, black_image(), (t_point){180, 58});
	print_text(&sdl, "POS:", (t_point){19, 137});
	print_text(&sdl, vector_string(t.location), (t_point){65, 137});
	print_text(&sdl, "SCALE:", (t_point){19, 98});
	print_text(&sdl, vector_string(t.scale), (t_point){80, 98});
}

static void	draw_current_operation(t_entity *ent, t_entity *collide, t_sdlcontext sdl)
{
	if (collide == NULL)
	{
		print_text(&sdl, "ADD:", (t_point){sdl.window_w / 2, sdl.window_h - 25});
		print_text(&sdl, ent->obj->name, (t_point){sdl.window_w / 2 + 60, sdl.window_h - 25});
	}
	else
	{
		print_text(&sdl, "DEL:", (t_point){sdl.window_w / 2, sdl.window_h - 25});
		print_text(&sdl, collide->obj->name, (t_point){sdl.window_w / 2 + 60, sdl.window_h - 25});
	}
}

static void findbounds(t_entity *ent)
{
	t_bound		zbound;
	t_vector3	v;
	int			i;

	zbound.max = -10000.0f;
	zbound.min= 10000.0f;
	i = 0;
	while (i < ent->obj->vertice_count)
	{
		v = ent->obj->vertices[i];
		if (v.z < zbound.min)
			zbound.min = v.z;
		if (v.z > zbound.max)
			zbound.max = v.z;
		i++;
	}
	ent->z_bound = zbound;
}

t_entity *selected_entity(t_editor *ed, t_sdlcontext sdl)
{
	t_list		*l;
	t_entity	*cur;
	
	l = ed->world.entitylist;
	while (l != NULL)
	{
		cur = l->content;
		if (entity_lookedat(ed, sdl, cur))
			return (cur);
		l = l->next;
	}
	return (NULL);
}

int	object_selector(t_editor *ed, t_sdlcontext sdl, int	original)
{
	int	i;

	i = 0;
	while (i < sdl.objectcount)
	{
		if (instant_text_button(sdl, &ed->mouse, sdl.objects[i].name, (t_point){20, 200 + (i * 20)}))
			return (i);
		i++;
	}
	return (original);
}

void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_entity	*ent;
	t_entity	*collide;

	ent = (t_entity *)ed->tool->tooldata;
	ent->object_index = object_selector(ed, sdl, ent->object_index);
	if (ent->obj != &sdl.objects[ent->object_index])
	{
		ent->obj = &sdl.objects[ent->object_index];
		findbounds(ent);
	}
	/* SPLIT HERE */
	ed->render.wireframe = true;
	ed->render.gizmocolor = CLR_GREEN;
	render_entity(sdl, &ed->render, ent);
	
	ed->render.wireframe = false;
	/* END SPLIT */
	draw_transform_info(ent->transform, sdl);
	if (instantbutton((t_rectangle) {30, 120, 20, 20}, &ed->mouse, sdl, "minus.png"))
		ent->transform.scale = vector3_add_xyz(ent->transform.scale, -0.25f);
	if (instantbutton((t_rectangle) {52, 120, 20, 20}, &ed->mouse, sdl, "one.png"))
		ent->transform.scale = vector3_one();
	if (instantbutton((t_rectangle) {74, 120, 20, 20}, &ed->mouse, sdl, "plus.png"))
		ent->transform.scale = vector3_add_xyz(ent->transform.scale, 0.25f);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		ent->transform.rotation.y += ed->mouse.scroll_delta * ft_degtorad(15.0f);
	else if ((ed->keystate >> KEYS_LALTMASK) & 1)
		ent->transform.rotation.z += ed->mouse.scroll_delta * ft_degtorad(15.0f);
	else
		ent->transform.rotation.x += ed->mouse.scroll_delta * ft_degtorad(15.0f);
	if (mouse_clicked(ed->mouse, MOUSE_MDL))
		ent->transform.rotation = vector3_zero();
	collide = selected_entity(ed, sdl);
	if (collide != NULL)
	{
		ed->render.gizmocolor = CLR_PRPL;
		ed->render.wireframe = true;
		render_entity(sdl, &ed->render, collide);
		entity_start_anim(collide, "walk");
		ed->render.wireframe = false;
		if (mouse_clicked(ed->mouse, MOUSE_RIGHT))
			list_remove(&ed->world.entitylist, collide, sizeof(t_entity));
	}
	if (mouse_clicked(ed->mouse, MOUSE_LEFT) && collide == NULL) //and selected is null, move to drawupdate
	{
		default_entity_occlusion_settings(ent, &ed->world);
		update_entity_bounds(ent);
		list_push(&ed->world.entitylist, ent, sizeof(t_entity));
	}
}

void	entity_tool_update(t_editor *ed)
{
	t_entity	*ent;
	t_vector3	dir;

	ent = (t_entity *)ed->tool->tooldata;
	dir = vector3_sub((t_vector3){ed->position.x, ed->position.y, 20.0f}, ent->transform.location);
	ent->transform.location = raycast(ed);//vector3_movetowards(ent->transform.location, dir, ed->clock.delta * 1.0f);
	ent->transform.location.z -= ent->z_bound.min * ent->transform.scale.z;
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
