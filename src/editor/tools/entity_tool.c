/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/01 13:30:16 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"

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
t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		NULL, NULL 
	};

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_entity));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
	}
	ft_strcpy(tool.icon_name, "entitytool.png");
	return (&tool);
}
