/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/02 16:47:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"
#include "file_io.h"
#include "inputhelp.h"
#include "objects.h"

static void	drawsquare(t_sdlcontext sdl, t_point crd, int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		draw(sdl, point_add(crd, (t_point){i, 0}), clr);
		draw(sdl, point_add(crd, (t_point){0, i}), clr);
		draw(sdl, point_add(crd, (t_point){TILESIZE, i}), clr);
		draw(sdl, point_add(crd, (t_point){TILESIZE, i}), clr);
		i++;
	}
}

static void	drawgrid(t_sdlcontext sdl, t_point origin)
{
	t_point	crd;

	crd.y = origin.y;
	while (crd.y < (TILESIZE * GRIDSIZE) + origin.y)
	{
		crd.x = origin.x;
		while (crd.x < (TILESIZE * GRIDSIZE) + origin.x)
		{
			drawsquare(sdl, crd, CLR_GRAY);
			crd.x += TILESIZE;
		}
		crd.y += TILESIZE;
	}
}

static void	update_render_editor(t_render *render, t_editor ed) //TODO: move game3d matrix stuff 
{
	render->position = (t_vector3){ed.offset.x, ed.offset.y - (ed.forward_offset.y * 75.0f), ed.offset.z};
	//render->lookdir = lookdirection(ed.lookangle);
	render->lookdir = (t_vector3){0.0f, -0.015f + ed.forward_offset.y, -1.0f};
}

static void	reload_objects(t_list **entitylist, t_sdlcontext sdl)
{
	t_list		*l;
	t_entity	*ent;

	l = *entitylist;
	while (l != NULL)
	{
		ent = (t_entity *)l->content;
		printf("reloaded object with %i object \n", ent->object_index);
		//ent->transform.location
		printf_vec(ent->transform.location);
		printf_vec(ent->transform.scale);
		ent->obj = &sdl.objects[ent->object_index]; //TODO: Protect!!s
		l = l->next;
	}
}

static void render_editor3d(t_sdlcontext sdl, t_editor ed)
{
	t_list	*l;
	t_entity	ent;

	l = ed.entitylist;
	while (l != NULL)
	{
		ent = *(t_entity *)l->content;
		render_object(sdl, ed.render, &ent);
		l = l->next;
	}
}

int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	t_gamereturn	gr;

	bzero(&ed, sizeof(t_editor));
	ed.linelist = load_chunk("map_test1", "WALL", sizeof(t_line));
	ed.buttonlist = load_chunk("buttons", "BUTN", sizeof(t_guibutton));
	initialize_buttons(ed.buttonlist, sdl);
	ed.tool = get_point_tool();
	gr = game_continue;
	ed.render = init_render(sdl);

	
	//DEBUGGING SKYBOX
	t_entity	skybox;
	ft_bzero(&skybox, sizeof(t_entity));
	skybox.obj = &sdl.objects[1];
	skybox.transform.scale = vector3_mul(vector3_one(), 20.0f);
	list_push(&ed.entitylist, &skybox, sizeof(t_entity));
	//

	while (gr == game_continue)
	{
		update_deltatime(&ed.clock);
		update_render_editor(&ed.render, ed);
		ft_bzero(sdl.surface->pixels, sizeof(uint32_t) * sdl.window_h * sdl.window_w);
		ft_bzero(sdl.zbuffer, sizeof(float) * sdl.window_h * sdl.window_w);
		gr = editor_events(&ed);
		render_editor3d(sdl, ed);
		draw_buttons(ed, sdl);
		if (ed.tool != NULL)
		{
			ed.tool->update(&ed);
			ed.tool->draw_update(&ed, sdl);
			if (ed.tool->icon != NULL) //Indicates which tool is selected
				draw_image(sdl, (t_point){ 8, sdl.window_h - 40 }, *ed.tool->icon, (t_point){32, 32});
			else if (ed.tool->icon_name[0] != '\0')
				ed.tool->icon = get_image_by_name(sdl, ed.tool->icon_name);
		}
		ed.mouse.click_unhandled = false;
		
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	save_lists_to_file(&ed);
	if (gr == game_exit)
		quit_game(&sdl);
	return (gr);
}