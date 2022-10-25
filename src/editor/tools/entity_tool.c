/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/25 14:44:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"



static void entity_tool_click(t_editor *ed)
{
	t_entity		*ent;

	ent = (t_entity *)ed->tool->tooldata;
	if (ed->mouse.click_unhandled && ed->mouse.click_button == MOUSE_LEFT) 
	{
		ent->position = point_to_vector2(ed->mouse.pos);
	}
	printf("entity tool click functionality is not implemented yet.\n");
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
