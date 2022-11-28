/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 15:49:59 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/28 19:41:53 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"
#include "objects.h"

static void int_slider_name(char *name, int *i, int mul, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	char		*str;
	t_rectangle	rect;
	float		moveamount;

	draw_text_boxed(&sdl, name, pos, sdl.screensize);
	moveamount = ed->hid.mouse.scroll_delta * mul;
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		moveamount *= 2;
	set_font_size(&sdl, 0);
	str = ft_itoa(*i);
	rect = draw_text_boxed(&sdl, str, (t_point) {pos.x + 100, pos.y }, sdl.screensize);
	if (pointrectanglecollision(ed->hid.mouse.pos, rect))
	{
		drawrectangle(sdl, rect, CLR_BLUE);
		if (moveamount != 0)
			drawrectangle(sdl, rect, CLR_GREEN);
		*i = *i + moveamount;
	}
	free(str);
}

static void float_slider_name(char *name, float *f, float mul, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	char		*str;
	t_rectangle	rect;
	float		moveamount;

	draw_text_boxed(&sdl, name, pos, sdl.screensize);
	moveamount = ed->hid.mouse.scroll_delta * mul;
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		moveamount *= 2.0f;
	if ((ed->hid.keystate >> KEYS_LALTMASK) & 1)
		moveamount *= 0.2f;
	set_font_size(&sdl, 0);
	str = ft_ftoa(*f, 8);
	rect = draw_text_boxed(&sdl, str, (t_point) {pos.x + 150, pos.y }, sdl.screensize);
	if (pointrectanglecollision(ed->hid.mouse.pos, rect))
	{
		drawrectangle(sdl, rect, CLR_BLUE);
		if (moveamount != 0)
			drawrectangle(sdl, rect, CLR_GREEN);
		*f = *f + moveamount;
	}
	free(str);
}

#include <stdlib.h>

static void float_slider(float *f, float mul, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	char		*str;
	t_rectangle	rect;
	float		moveamount;

	moveamount = ed->hid.mouse.scroll_delta * mul;
	if ((ed->hid.keystate >> KEYS_SHIFTMASK) & 1)
		moveamount *= 2.0f;
	if ((ed->hid.keystate >> KEYS_LALTMASK) & 1)
		moveamount *= 0.2f;
	set_font_size(&sdl, 0);
	str = ft_ftoa(*f, 8);
	rect = draw_text_boxed(&sdl, str, pos, sdl.screensize);
	if (pointrectanglecollision(ed->hid.mouse.pos, rect))
	{
		drawrectangle(sdl, rect, CLR_BLUE);
		if (moveamount != 0)
			drawrectangle(sdl, rect, CLR_GREEN);
		*f = *f + moveamount;
	}
	free(str);
}

static void vector3_slider(char *name, t_vector3 *vec, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	draw_text_boxed(&sdl, name, pos, sdl.screensize);
	float_slider(&vec->x, 1.0f, ed, sdl, (t_point) {pos.x, pos.y + 20} );
	float_slider(&vec->y, 1.0f, ed, sdl, (t_point) {pos.x + 60, pos.y + 20} );
	float_slider(&vec->z, 1.0f, ed, sdl, (t_point) {pos.x + 120, pos.y + 20} );
}

static void bool_toggle(char *name, bool *b, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	draw_text_boxed(&sdl, name, pos, sdl.screensize);
	if (instant_text_button(sdl, &ed->hid.mouse, "on", (t_point){pos.x + 140, pos.y}))
		*b = true;
	if (instant_text_button(sdl, &ed->hid.mouse, "off", (t_point){pos.x + 180, pos.y}))
		*b = false;
}


t_object	*object_selector2(t_editor *ed, t_sdlcontext sdl, t_entity *original)
{
	int			i;
	char	str[256] = { };

	i = 0;
	while (i < sdl.objectcount)
	{
		if (instant_text_button(sdl, &ed->hid.mouse, sdl.objects[i].name, (t_point){20, 200 + (i * 20)}))
		{
			ft_strcpy(original->object_name, sdl.objects[i].name);
			return (&sdl.objects[i]);
		}
		i++;
	}
	return (original->obj);
}

void	string_box(char *name, char *str, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	SDL_Event	e;
	bool		done;
	t_rectangle	rect;
	t_point		start;

	done = false;
	rect = draw_text_boxed(&sdl, name, pos, sdl.screensize);
	start = point_add(rect.position, (t_point){rect.size.x + 10, 2});
	if (instant_text_button(sdl, &ed->hid.mouse, str, start))
	{
		SDL_StartTextInput();
		while (!done)
		{
			while (SDL_PollEvent(&e) && !done)
			{
				screen_blank(sdl);
				draw_text_boxed(&sdl, name, pos, sdl.screensize);
				if (e.type == SDL_TEXTINPUT && ft_strlen(str) < 31)
				{
					ft_strncat(str, e.text.text, 32);
					
				}
				if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE)
					str[ft_strlen(str) - 1] = '\0';
				rect = draw_text_boxed(&sdl, str, start, sdl.screensize);
				drawrectangle(sdl, rect, CLR_GREEN);
				SDL_UpdateWindowSurface(sdl.window);
				if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2)) {
					done = true;
				}
					
			}
		}
	}
}

void	list_gun_presets(t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	t_list	*l;
	t_gun	*gun;
	char	str[256];
	int		i;

	l = ed->world.guns;
	draw_text_boxed(&sdl, "Gun presets:", pos, sdl.screensize);
	i = 1;
	while (l != NULL)
	{
		gun = l->content;
		ft_bzero(str, sizeof(str));
		sprintf(str, "%s (model: %s)", gun->preset_name, gun->entity.object_name);
		if (instant_text_button(sdl, &ed->hid.mouse, str, point_add(pos, (t_point){10, i * 20})))
		{
			ed->player.gun = gun;
			ed->player.gun->entity.transform.parent = &ed->player.transform;
			ed->player.gun->readytoshoot = true;
		}
		if (instant_text_button(sdl, &ed->hid.mouse, "-", point_add(pos, (t_point){0, i * 20})))
			list_remove(&ed->world.guns, gun, sizeof(t_gun));
		i++;
		l = l->next;
	}
}

void	save_preset(t_editor *ed, t_sdlcontext sdl)
{
	t_list	*l;
	t_gun	*gun;
	char	str[256] = { };

	l = ed->world.guns;
	while (l != NULL)
	{
		gun = l->content;
		printf("cmp '%s' '%s' \n", gun->preset_name, ed->player.gun->preset_name);
		if (ft_strcmp(gun->preset_name, ed->player.gun->preset_name) == 0)
		{
			gun = ed->player.gun;
			sprintf(str, "Overwrote preset %s", gun->preset_name);
			debugconsole_addmessage(&ed->world.debugconsole, str);
			return ;
		}
		l = l->next;
	}
	list_push(&ed->world.guns, ed->player.gun, sizeof(t_gun));
	sprintf(str, "Saved new preset %s", ed->player.gun->preset_name);
	debugconsole_addmessage(&ed->world.debugconsole, str);
}

void	gun_tool_draw(t_editor *ed, t_sdlcontext sdl)
{
	t_guntooldata	*dat;

	dat = (t_guntooldata *)ed->tool->tooldata;
	ed->player.gun->disabled = false;
	if (instant_text_button(sdl, &ed->hid.mouse, "Model", (t_point) {20, 140}))
		dat->gtm = gtm_model;
	if (instant_text_button(sdl, &ed->hid.mouse, "Offset", (t_point) {80, 140}))
		dat->gtm = gtm_offset;
	if (instant_text_button(sdl, &ed->hid.mouse, "Recoil", (t_point) {140, 140}))
		dat->gtm = gtm_recoil;
	if (dat->gtm == gtm_model)
	{
		ed->player.gun->entity.obj = object_selector2(ed, sdl, &ed->player.gun->entity);
	}
	if (dat->gtm == gtm_offset)
	{
		vector3_slider("aiming  position", &ed->player.gun->aimpos, ed, sdl, (t_point) {20, sdl.window_h / 2 });
		vector3_slider("holster position", &ed->player.gun->holsterpos, ed, sdl, (t_point) {20, (sdl.window_h / 2) + 60});
	}
	if (dat->gtm == gtm_recoil)
	{
		float_slider_name("angle y", &ed->player.gun->recoilangle.y, 0.5f, ed, sdl, (t_point) {20, 200});
		float_slider_name("jump y", &ed->player.gun->recoiljump.y ,0.1f, ed, sdl, (t_point) {20, 220});
		float_slider_name("viewjump y", &ed->player.gun->viewrecoil.y , 0.0005f, ed, sdl, (t_point) {20, 240});
		float_slider_name("angle recovery", &ed->player.gun->anglerecovery, 0.0001f, ed, sdl, (t_point) {20, 260});
		bool_toggle("auto", &ed->player.gun->fullauto, ed, sdl, (t_point) {20, (sdl.window_h / 2) + 100});
		int_slider_name("firedelay", &ed->player.gun->firedelay, 15, ed, sdl, (t_point) {20, (sdl.window_h / 2) + 140});
	}
	string_box("Preset name:", ed->player.gun->preset_name, ed, sdl, (t_point) {20, 80});
	if (instant_text_button(sdl, &ed->hid.mouse, "Save", (t_point){20, 100}))
		save_preset(ed, sdl);
	list_gun_presets(ed, sdl, (t_point){20, 540});
}

void	gun_tool_cleanup(t_editor *ed, t_sdlcontext sdl)
{
	ed->player.gun->disabled = true;
}

t_tool	*get_gun_tool()
{
	static t_tool	tool
	= {
		gun_tool_draw, gun_tool_cleanup
	};
	t_guntooldata	*dat;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_guntooldata));
	}
	ft_strcpy(tool.icon_name, "linetool.png");
	return (&tool);
}
