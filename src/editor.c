/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:47:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/21 20:33:43 by raho             ###   ########.fr       */
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
	render->position = ed.position;
	render->lookdir = ed.forward;
	//render->matcamera = matrix_lookat(render->position, render->vtarget, (t_vector3){0, 0, 1});
	//render->matview = matrix_quickinverse(render->matcamera);
}

static void	join_surfaces(SDL_Surface *base, SDL_Surface *new, t_point pos)
{
	t_point	i;

	i.y = 0;
	while (i.y < new->h)
	{
		i.x = 0;
		while (i.x < new->w)
		{
			if (((uint32_t *)new->pixels)[i.x + (i.y * new->w)] >> 24 != 0)
				((uint32_t *)base->pixels)[(i.x + pos.x) + ((i.y + pos.y) * base->w)] = ((uint32_t *)new->pixels)[i.x + (i.y * new->w)];
			i.x++;
		}
		i.y++;
	}
}

void	print_ttftext(t_sdlcontext *sdl, const char *text, t_point pos)
{
	SDL_Surface	*surfacetext;

	surfacetext = TTF_RenderText_Blended(sdl->ttfont->font, text, sdl->ttfont->color);
	if (!surfacetext)
		error_log(EC_TTF_RENDERTEXTBLENDED);
	join_surfaces(sdl->surface, surfacetext, pos);
	SDL_FreeSurface(surfacetext);
}


int	editorloop(t_sdlcontext sdl)
{
	t_editor		ed;
	int				i;

	bzero(&ed, sizeof(t_editor));
	ed.buttonlist = load_chunk("buttons", "BUTN", sizeof(t_guibutton));
	initialize_buttons(ed.buttonlist, sdl);
	ed.world = load_world("world1", sdl);
	ed.tool = get_wall_tool();
	ed.gamereturn = game_continue;
	ed.render = init_render(sdl);
	//ed.angle = (t_vector2){-RAD90, -RAD90 * 0.99f};
	ed.angle = (t_vector2){-20.0f, -RAD90 * 0.99f};
	ed.position = (t_vector3){500.0f, 500.0f, 200.0f};
	set_font_size(&sdl, 0);
	i = 0;
	while (ed.gamereturn == game_continue)
	{
		update_deltatime(&ed.clock);
		ed.gamereturn = editor_events(&ed);
		move_editor(&ed);
		update_render_editor(&ed.render, ed);
		screen_blank(sdl);
		render_start(&ed.render);
		render_world3d(sdl, ed.world, &ed.render);
		if (ed.tool != NULL)
		{
			ed.tool->draw_update(&ed, sdl); //Instant buttons here can toggle mouse.click unhandled, so draw first
			ed.tool->update(&ed);
			if (ed.tool->icon != NULL) //Indicates which tool is selected
				draw_image(sdl, (t_point){ 8, sdl.window_h - 40 }, *ed.tool->icon, (t_point){32, 32});
			else if (ed.tool->icon_name[0] != '\0')
				ed.tool->icon = get_image_by_name(sdl, ed.tool->icon_name);
		}
		draw_buttons(ed, sdl);
		ed.mouse.click_unhandled = false;
		draw_text_boxed(&sdl, "tab to unlock/lock mouse, shift + enter to go to playmode", (t_point){sdl.window_w / 2, 10}, (t_point){sdl.window_w, sdl.window_h});
		char *fps = ft_itoa(ed.clock.fps);
		draw_text_boxed(&sdl, fps, (t_point){sdl.window_w - 80, 10}, (t_point){sdl.window_w, sdl.window_h});

		sdl.ttfont->color = sdl.ttfont->font_colors.black;
		sdl.ttfont->font = sdl.ttfont->font_sizes[0];
		print_ttftext(&sdl, "coming in hot guns blazing", (t_point){10, 100});
		sdl.ttfont->font = sdl.ttfont->font_sizes[1];
		print_ttftext(&sdl, "coming in hot guns blazing", (t_point){10, 200});
		sdl.ttfont->font = sdl.ttfont->font_sizes[2];
		print_ttftext(&sdl, "coming in hot guns blazing", (t_point){10, 300});
		sdl.ttfont->font = sdl.ttfont->font_sizes[3];
		print_ttftext(&sdl, "coming in hot guns blazing", (t_point){10, 400});
		
		free(fps);
		if (SDL_UpdateWindowSurface(sdl.window) < 0)
			error_log(EC_SDL_UPDATEWINDOWSURFACE);
	}
	save_world("world1", ed.world);
	save_editordata(&ed);
	free_render(ed.render);
	if (ed.gamereturn == game_exit)
		quit_game(&sdl);
	return (ed.gamereturn);
}