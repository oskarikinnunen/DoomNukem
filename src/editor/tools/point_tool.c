/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_tool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 15:05:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 18:35:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

#include "stdio.h"

static t_point	line_dir(t_line line)
{
	return (point_sub(line.start, line.end));
}

static bool	is_legal_line(t_editor *ed) //Only kinda protects the user from drawing overlapping lines
{
	/*t_list		*l;
	t_line		cur;
	t_vector2	cur_dir;
	t_vector2	ed_dir;

	l = ed->linelist;
	while (l != NULL)
	{
		cur = *(t_line *)l->content;
		cur_dir = vector2_normalise(point_to_vector2(line_dir(cur)));
		ed_dir = vector2_normalise(point_to_vector2(line_dir(ed->line)));
		if (point_cmp(ed->line.start, ed->line.end) == true)
			return (false);
		if ((point_cmp(ed->line.start, cur.start)
			|| point_cmp(ed->line.start, cur.end))
			&& vector2_cmp(ed_dir, cur_dir) == true)
			{
				return (false);
			}
		l = l->next;
	}
	return (true);*/
}

static void point_tool_click(t_editor *ed)
{
	/*static t_point_tool_state	state;

	ed->mouse.click_unhandled = false;
	if (ed->mouse.click_button == MOUSE_LEFT)
	{
		if (state == place_start)
		{
			ed->line.start = mousetogridspace(ed);
			state = place_end;
		}
		else if (state == place_end) 
		{
			ed->line.end = mousetogridspace(ed);
			if (is_legal_line(ed) == true)
				list_push(&ed->linelist, &ed->line, sizeof(t_line));
			ed->line.start = point_zero();
			ed->line.end = point_zero();
			state = place_start;
		}
	}*/
}


static void	render_cursorball(t_editor ed, t_sdlcontext sdl)
{
	t_point	circlepos;

	circlepos = mousetogridspace(&ed);
	circlepos = point_mul(circlepos, TILESIZE);
	//circlepos = point_add(circlepos, (t_point)ed.offset);
	drawcircle(sdl, circlepos, 10, CLR_TURQ);
}

static void	point_tool_draw(t_editor *ed, t_sdlcontext sdl) //This needs to access editors state, so pass editor here??
{
	return ;
}

static void	point_tool_update(t_editor *ed) //This needs to access editors state, so pass editor here??
{
	/*if (ed->mouse.click_unhandled)
		point_tool_click(ed);*/
	return ;
}

t_tool	*get_point_tool()
{
	static t_tool	tool
	= {
		point_tool_update, point_tool_draw
	};

	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
