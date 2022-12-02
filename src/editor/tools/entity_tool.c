/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/02 15:58:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "tools/entitytool.h" 
#include "objects.h"

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
	draw_text_boxed(&sdl, vector_string(t.position), (t_point){65, 143}, (t_point){sdl.window_w, sdl.window_h});
	draw_text_boxed(&sdl, "SCALE:", (t_point){20, 105}, (t_point){sdl.window_w, sdl.window_h});
	draw_text_boxed(&sdl, vector_string(t.scale), (t_point){65, 105}, (t_point){sdl.window_w, sdl.window_h});
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
		if (instant_text_button(sdl, &ed->hid.mouse, sdl.objects[i].name, (t_point){20, 200 + (i * 20)}))
			return (i);
		i++;
	}
	return (original);
}

static void	entity_tool_lazyinit(t_editor *ed, t_sdlcontext *sdl, t_entitytooldata *dat)
{
	if (dat->objectgui.gui.sdl == NULL)
	{
		dat->objectgui.gui = init_gui(sdl, &ed->hid, &ed->player, (t_point) {20, 40}, "Object selector");
		dat->objectgui.autoclose = false;
	}
}

void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_entitytooldata	*dat;
	t_entity			*ent;
	t_entity			*collide;

	dat = (t_entitytooldata *)ed->tool->tooldata;
	ent = dat->ent;
	entity_tool_lazyinit(ed, &sdl, dat);
	objectgui_update(&dat->objectgui, ent);
	
	ent->transform.position = raycast(ed);//vector3_movetowards(ent->transform.location, dir, ed->clock.delta * 1.0f);
	ent->transform.position.z -= ent->z_bound.min * ent->transform.scale.z;
	/*ent->object_index = object_selector(ed, sdl, ent->object_index);
	if (ent->obj != &sdl.objects[ent->object_index])
	{
		ent->obj = &sdl.objects[ent->object_index];
		findbounds(ent);
	}*/
	ed->render.wireframe = true;
	ed->render.gizmocolor = CLR_GREEN;
	render_entity(sdl, ed->render, ent);
	ed->render.wireframe = false;
	/* END SPLIT */
	/*set_font_size(&sdl, 0);
	draw_transform_info(ent->transform, sdl);
	if (instantbutton((t_rectangle) {30, 120, 20, 20}, &ed->hid.mouse, sdl, "minus.png"))
		ent->transform.scale = vector3_add_xyz(ent->transform.scale, -0.25f);
	if (instantbutton((t_rectangle) {52, 120, 20, 20}, &ed->hid.mouse, sdl, "one.png"))
		ent->transform.scale = vector3_one();
	if (instantbutton((t_rectangle) {74, 120, 20, 20}, &ed->hid.mouse, sdl, "plus.png"))
		ent->transform.scale = vector3_add_xyz(ent->transform.scale, 0.25f);
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		ent->transform.rotation.y += ed->hid.mouse.scroll_delta * ft_degtorad(15.0f);
	else if ((ed->hid.keystate >> KEYS_LALTMASK) & 1)
		ent->transform.rotation.z += ed->hid.mouse.scroll_delta * ft_degtorad(15.0f);
	else
		ent->transform.rotation.x += ed->hid.mouse.scroll_delta * ft_degtorad(15.0f);
	if (mouse_clicked(ed->hid.mouse, MOUSE_MDL))
		ent->transform.rotation = vector3_zero();
	collide = selected_entity(ed, sdl);
	if (collide != NULL)
	{
		ed->render.gizmocolor = CLR_PRPL;
		ed->render.wireframe = true;
		render_entity(sdl, ed->render, collide);
		entity_start_anim(collide, "walk");
		ed->render.wireframe = false;
		if (mouse_clicked(ed->hid.mouse, MOUSE_RIGHT))
			list_remove(&ed->world.entitylist, collide, sizeof(t_entity));
	}
	if (mouse_clicked(ed->hid.mouse, MOUSE_LEFT) && collide == NULL) //and selected is null, move to drawupdate
		list_push(&ed->world.entitylist, ent, sizeof(t_entity));*/
}

t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		entity_tool_draw 
	};
	t_entitytooldata	*dat; //TODO: make entity tool use it's own tooldata struct,

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_entitytooldata));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
		dat = (t_entitytooldata *)tool.tooldata;
		dat->ent = ft_memalloc(sizeof(t_entity));
		dat->ent->transform.scale = vector3_one();
		dat->ent->object_index = 1;
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
