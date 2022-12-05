/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_common_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 13:54:44 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/05 17:19:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "objects.h"
#include "editor_tools.h"


void int_slider_name(char *name, int *i, int mul, t_editor *ed, t_sdlcontext sdl, t_point pos)
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

void float_slider_name(char *name, float *f, float mul, t_editor *ed, t_sdlcontext sdl, t_point pos)
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

void float_slider(float *f, float mul, t_editor *ed, t_sdlcontext sdl, t_point pos)
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

void vector3_slider(char *name, t_vector3 *vec, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	draw_text_boxed(&sdl, name, pos, sdl.screensize);
	float_slider(&vec->x, 1.0f, ed, sdl, (t_point) {pos.x, pos.y + 20} );
	float_slider(&vec->y, 1.0f, ed, sdl, (t_point) {pos.x + 60, pos.y + 20} );
	float_slider(&vec->z, 1.0f, ed, sdl, (t_point) {pos.x + 120, pos.y + 20} );
}

void bool_toggle(char *name, bool *b, t_editor *ed, t_sdlcontext sdl, t_point pos)
{
	draw_text_boxed(&sdl, name, pos, sdl.screensize);
	if (instant_text_button(sdl, &ed->hid.mouse, "on", (t_point){pos.x + 140, pos.y}))
		*b = true;
	if (instant_text_button(sdl, &ed->hid.mouse, "off", (t_point){pos.x + 180, pos.y}))
		*b = false;
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