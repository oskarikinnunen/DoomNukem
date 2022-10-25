/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/25 17:14:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"

void entity_tool_append_list(t_editor *ed, t_entity ent)
{
	if (ed->entitylist == NULL)
	{
		ed->entitylist = ft_lstnew(&ent, sizeof(t_entity));
	}
	ft_memcpy(ed->entitylist->content, &ent, sizeof(t_entity));
}

static void entity_tool_click(t_editor *ed)
{
	t_entity		*ent;

	ent = (t_entity *)ed->tool->tooldata;
	if (ed->mouse.click_unhandled && ed->mouse.click_button == MOUSE_LEFT) 
	{
		ent->position = point_to_vector2(point_sub(ed->mouse.pos, ed->offset));
		entity_tool_append_list(ed, *ent);
		printf("entity UID %i placed to %f %f \n", ent->id, ent->position.x, ent->position.y);
		save_entitylist(ed->entitylist, "entitylist");
	}
	ed->mouse.click_unhandled = false;
}

static void	entity_tool_draw(t_editor *ed, t_sdlcontext sdl) //This needs to access editors state, so pass editor here??
{
	static t_img	entity_img;
	t_entity		*ent;

	ent = (t_entity *)ed->tool->tooldata;
	if (entity_img.length == 0)
		entity_img = get_image_by_name(sdl, "images/entitytool.png");
	draw_image(sdl, ed->mouse.pos, entity_img, entity_img.size);
	draw_image(sdl, point_add(ed->offset, vector2_to_point(ent->position)), entity_img, entity_img.size);
	return ;
}

static void	entity_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	if (ed->mouse.click_unhandled)
		entity_tool_click(ed);
}

t_tool	*get_entity_tool()
{
	static t_tool	tool
	= {
		entity_tool_update, entity_tool_draw
	};

	if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
		SDL_ShowCursor(SDL_DISABLE);
	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_entity));
		if (tool.tooldata == NULL)
			error_log(EC_MALLOC);
	}
	return (&tool);
}
