/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 14:37:27 by okinnune         ###   ########.fr       */
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

void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_entity	*ent;

	ent = (t_entity *)ed->tool->tooldata;
	ent->object_index = ft_clamp(ent->object_index, 0, sdl.objectcount - 1);
	if (ent->obj != &sdl.objects[ent->object_index])
		ent->obj = &sdl.objects[ent->object_index];
	if (ent->obj == NULL)
		ent->obj = object_plane();
	render_object(sdl, ed->render, ent);
}


void	entity_tool_update(t_editor *ed)
{
	t_entity	*ent;
	t_vector3	dir;

	ent = (t_entity *)ed->tool->tooldata;
	//ent->transform.location = (t_vector3){ed->offset.x, ed->offset.y, 0};
	dir = vector3_sub((t_vector3){ed->offset.x, ed->offset.y, 0}, ent->transform.location);
	ent->transform.location = vector3_movetowards(ent->transform.location, dir, ed->clock.delta * 0.05f);
	if ((ed->keystate >> KEYS_SHIFTMASK) & 1)
		ent->object_index += ed->mouse.scroll_delta;
	if (mouse_clicked(ed->mouse, MOUSE_LEFT))
	{
		list_push(&ed->entitylist, ent, sizeof(t_entity));
	}
		/*ed->reload_objects = true;
		ft_bzero(&ent, sizeof(t_entity));
		ent.obj = NULL;
		printf("ENTITY TOOL CLICK!!\n");
		ent.object_index = 1;
		ent.transform.scale = vector3_one();
		ent.transform.location = (t_vector3){ed->offset.x, -ed->offset.y, ed->offset.z};*/
		//list_push(&ed->entitylist, ent, sizeof(t_entity));
		//printf("after list push, list len is %i \n", ft_listlen(ed->entitylist));
		
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
