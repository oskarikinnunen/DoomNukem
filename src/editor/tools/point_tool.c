/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_tool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/25 14:27:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

#include "stdio.h"

static void point_tool_click(t_editor *ed)
{
	static t_point_tool_state	state;

	ed->mouse.click_unhandled = false;
	if (ed->mouse.click_button == MOUSE_LEFT)
	{
		if (state == place_start)
		{
			ed->line.start = mousetoworldspace(ed);
			state = place_end;
		}
		else if (state == place_end) 
		{
			//Check here that we don't cross previous line
			
			ed->line.end = mousetoworldspace(ed);
			state = place_start;
			saveline(ed);
			ed->line.start = point_zero();
			ed->line.end = point_zero();
		}
		//place line, change state
	}
	else if (ed->mouse.click_button == MOUSE_RIGHT)
	{
		if (state == place_end)
		{
			state = place_start;
			ed->line.start = point_zero();
			ed->line.end = point_zero();
		}
		else
			point_tool_delete(ed, mousetoworldspace(ed));
		//If we haven't started placing a line, try to remove existing lines from ed linelist, don't change state
		//Otherwise just delete the line we're currently placing and change state to place_start
	}
}

static void	point_tool_draw(t_editor *ed, t_sdlcontext sdl) //This needs to access editors state, so pass editor here??
{
	t_line			ss_line;

	if (point_cmp(ed->line.end, point_zero()) && !point_cmp(ed->line.start, point_zero()))
	{
		ss_line.start = point_mul(ed->line.start, TILESIZE);
		ss_line.start = point_add(ss_line.start, ed->offset);
		
		ss_line.end = ed->mouse.pos;
		drawline(sdl, ss_line.start, ss_line.end, CLR_TURQ);
	}
	return ;
}

static void	point_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	if (ed->mouse.click_unhandled)
		point_tool_click(ed);
	return ;
}

static void	point_tool_key(int key) //make static?
{
	return ;
}

t_tool	*get_point_tool()
{
	static t_tool	tool
	= {
		point_tool_update, point_tool_draw
	};

	return (&tool);
}
