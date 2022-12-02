/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autogui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 16:19:23 by okinnune          #+#    #+#             */
/*   Updated: 2022/12/02 15:58:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

t_autogui	init_gui(t_sdlcontext *sdl, t_hid_info *hid, t_player *player, t_point origin, char *title)
{
	t_autogui	gui;

	ft_bzero(&gui, sizeof(t_autogui));
	gui.sdl = sdl;
	gui.hid = hid;
	gui.player = player;
	gui.rect.position = origin;
	gui.rect.size = (t_point){240, 400};
	gui.title = title;
	return (gui);
}

void	gui_dock(t_autogui *gui, t_autogui *dockto)
{
	gui->dock = dockto;
}

static t_rectangle	empty_rect()
{
	return ((t_rectangle){0});
}

static void gui_limitrect(t_autogui *gui)
{
	gui->rect.size.x = ft_clamp(gui->rect.size.x, 240, 600);
	gui->rect.size.y = ft_clamp(gui->rect.size.y, 128, gui->sdl->screensize.y);
	gui->rect.position.x = ft_clamp(gui->rect.position.x, 6, gui->sdl->window_w - 6 - gui->rect.size.x);
	gui->rect.position.y = ft_clamp(gui->rect.position.y, 6, gui->sdl->window_h - 6 - gui->rect.size.y);
}

void	gui_start(t_autogui *gui)
{
	if (!gui->hidden)
	{
		draw_rectangle_filled(*gui->sdl, gui->rect, CLR_DARKGRAY);
		drawrectangle(*gui->sdl, gui->rect, CLR_DARKGRAY);
	}
	//gui->scrollable = false;
	gui->overdraw = 0;
	gui->offset.y = 32 + 5; //TODO: autogui_start
	gui->offset.x = 5;
	gui_limitrect(gui);
}

static void update_scrollbar(t_autogui *gui)
{
	t_rectangle	scrollrect;
	int			max_y;
	float		lerp;
	int			y_off;

	scrollrect.size = (t_point){16, 16};
	scrollrect.position = point_add(gui->rect.position, (t_point){.x = gui->rect.size.x});
	scrollrect.position.y += 32;
	scrollrect.position.x -= scrollrect.size.x;
	max_y = gui->rect.size.y - 64 - scrollrect.size.x;
	lerp = 0.0f;
	if (-gui->scroll.y != 0)
		lerp = (float)-gui->scroll.y / (float)gui->overdraw;
	scrollrect.position.y += (float)max_y * lerp;
	draw_rectangle_filled(*gui->sdl, scrollrect, INT_MAX);
	drawrectangle(*gui->sdl, scrollrect, CLR_BLUE);
	if (gui->hid->mouse.held == MOUSE_LEFT && pointrectanglecollision(gui->hid->mouse.pos, scrollrect))
		gui->scroll_held = true;
	if (pointrectanglecollision(gui->hid->mouse.pos, scrollrect))
		drawrectangle(*gui->sdl, scrollrect, CLR_GREEN);
	if (gui->scroll_held)
		gui->scroll.y += (scrollrect.position.y + (scrollrect.size.y / 2)) - gui->hid->mouse.pos.y;
}

static void draw_rect_tri(t_sdlcontext *sdl, t_rectangle rect, uint32_t clr)
{
	t_point	p;
	int		linelen;
	draw_triangle(sdl, point_add(rect.position, (t_point){.y = rect.size.y}),
			point_add(rect.position, rect.size), point_add(rect.position, (t_point){.x = rect.size.x}), clr);
	p = (t_point){0, 0};
	linelen = 0;
	while (p.y < rect.size.y)
	{
		p.x = linelen;
		while (p.x > 0)
		{
			if (p.x % 2 == 0 || p.y % 2 == 0)
				draw(*sdl, point_add(rect.position, (t_point){-p.x + rect.size.x, p.y}), INT_MAX);
			p.x--;
		}
		linelen++;
		p.y++;
	}
}

static bool	clicked_rectangle(t_mouse m, t_rectangle rect)
{
	return (mouse_clicked(m, MOUSE_LEFT) && pointrectanglecollision(m.pos, rect));
}

static void update_hidecross(t_autogui *gui)
{
	t_rectangle	hidecross;
	t_point		p1;
	t_point		p2;
	hidecross.size = (t_point){32, 32};
	hidecross.position = point_add(gui->rect.position, (t_point){.x = gui->rect.size.x});
	hidecross.position.x -= hidecross.size.x;
	draw_rectangle_filled(*gui->sdl, hidecross, INT_MAX);
	drawrectangle(*gui->sdl, hidecross, CLR_BLUE);
	p1 = hidecross.position;
	p2 = point_add(hidecross.position, hidecross.size);
	drawline(*gui->sdl, p1, p2, CLR_RED);
	p1 = point_add(hidecross.position, (t_point){.x = hidecross.size.x});
	p2 = point_add(hidecross.position, (t_point){.y = hidecross.size.y});
	drawline(*gui->sdl, p1, p2, CLR_RED);
	drawline(*gui->sdl, hidecross.position, point_add(hidecross.position, hidecross.size), CLR_RED);
	if (clicked_rectangle(gui->hid->mouse, hidecross))
		gui->hidden = true;
}

void	gui_update(t_autogui *gui)
{
	t_rectangle	dragcorner;
	t_rectangle	dragbar;

	if (gui->hidden)
		return ;
	dragbar = gui->rect;
	dragbar.size.y = 32;
	draw_rectangle_filled(*gui->sdl, dragbar, INT_MAX);
	if (gui->dock != NULL)
	{
		gui->rect.position = point_add(gui->dock->rect.position, (t_point){.x = gui->dock->rect.size.x + 2});
		drawrectangle(*gui->sdl, dragbar, CLR_DARKGRAY);
	}
	else
	{
		drawrectangle(*gui->sdl, dragbar, CLR_BLUE);
		if (pointrectanglecollision(gui->hid->mouse.pos, dragbar) || gui->move_held)
			drawrectangle(*gui->sdl, dragbar, CLR_GREEN);
		if (gui->hid->mouse.held == MOUSE_LEFT && pointrectanglecollision(gui->hid->mouse.pos, dragbar))
			gui->move_held = true;
		if (gui->move_held)
			gui->rect.position = point_sub(gui->hid->mouse.pos, point_div(dragbar.size, 2));
	}
	if (gui->allow_user_hide)
	{
		update_hidecross(gui);
	}
	dragcorner.size = (t_point) {16,16};
	dragcorner.position = point_sub(point_add(gui->rect.position, gui->rect.size), dragcorner.size);
	//draw_rectangle_filled(*gui->sdl, dragcorner, INT_MAX);
	//drawrectangle(*gui->sdl, dragcorner, CLR_BLUE);
	draw_rect_tri(gui->sdl, dragcorner, CLR_BLUE);
	if (pointrectanglecollision(gui->hid->mouse.pos, dragcorner) || gui->drag_held)
		draw_rect_tri(gui->sdl, dragcorner, CLR_GREEN);
		
	if (gui->hid->mouse.held == MOUSE_LEFT && pointrectanglecollision(gui->hid->mouse.pos, dragcorner))
		gui->drag_held = true;
	if (gui->drag_held)
	{
		gui->scroll.y = 0;
		gui->rect.size = point_sub(point_add(gui->hid->mouse.pos, point_div(dragcorner.size, 2)), gui->rect.position);
	}
	gui_limitrect(gui);
	if (gui->hid->mouse.held == 0)
	{
		gui->move_held = false;
		gui->drag_held = false;
		gui->scroll_held = false;
		if (gui->locking_player)
		{
			gui->locking_player = false;
			gui->player->locked = false;
			gui->hid->mouse.safe_delta = false;
			force_mouseunlock(gui->hid);
		}
	}
	if (gui->offset.y < gui->rect.size.y)
	{
		gui->scroll.y = 0;
		gui->scrollable = false;
	}
	if (gui->scrollable)
	{
		gui->scroll.y += gui->hid->mouse.scroll_delta * 16;
		update_scrollbar(gui);
		gui->scroll.y = ft_clamp(gui->scroll.y, -gui->overdraw, 0);
	}
	set_font_size(gui->sdl, 2);
	draw_text_boxed(gui->sdl, gui->title, gui->rect.position, gui->sdl->screensize);
	set_font_size(gui->sdl, 0);
}

void	objectgui_update(t_objectgui *ogui, t_entity *ent)
{
	t_object	*obj;
	t_autogui	*gui;
	int			i;

	gui = &ogui->gui;
	gui_start(gui);
	i = 0;
	while (i < gui->sdl->objectcount)
	{
		obj = &gui->sdl->objects[i];
		if (gui_button(obj->name, gui))
		{
			ent->obj = obj;
			if (ogui->autoclose)
				gui->hidden = true;
		}
		i++;
	}
	gui_update(gui);
}

static t_point gui_currentpos(t_autogui *gui)
{
	return (point_add(gui->rect.position, point_add(gui->scroll, gui->offset)));
}

static bool gui_shoulddraw(t_autogui *gui)
{
	t_point	cp;

	if (gui->hidden)
		return (false);
	cp = gui_currentpos(gui);
	if (cp.y > gui->rect.position.y + 32 && cp.y + 20 < gui->rect.position.y + gui->rect.size.y)
		return (true);
	gui->scrollable = true;
	/*if (cp.y + 20 > gui->rect.position.y + gui->rect.size.y)
		gui->overdraw += 20;*/
	return (false);
}

void	gui_starthorizontal(t_autogui *gui)
{
	//TODO: check if already horizontal??
	gui->agl = agl_horizontal;
	gui->offset.x = 0;
}

void	gui_endhorizontal(t_autogui *gui)
{
	gui->agl = agl_vertical;
	gui->offset.x = 0;
	gui->offset.y += 20;
}

//Internal function, rename with a better name
static	void	gui_layout(t_autogui *gui, t_rectangle rect)
{
	int	x;

	x = ft_max(rect.size.x + 5, gui->min_x);
	if (gui->agl == agl_vertical)
	{
		if (!gui_shoulddraw(gui))
		{
			gui->overdraw += 20;
		}
		gui->offset.y += 20;
	}
	else
		gui->offset.x += x;
}

void	gui_labeled_float_slider(char *str, float *f, float mul, t_autogui *gui)
{
	gui_starthorizontal(gui);
	gui_label(str, gui);
	gui_float_slider(f, mul, gui);
	gui_endhorizontal(gui);
}

void	gui_label(char *str, t_autogui *gui)
{
	t_rectangle	rect;

	rect = empty_rect();
	if (gui_shoulddraw(gui)) 
	{
		rect = draw_text_boxed(gui->sdl, str, gui_currentpos(gui), gui->rect.size);
	}
	gui_layout(gui, rect);
}

void	gui_float_slider(float	*f, float mul, t_autogui *gui)
{
	t_rectangle	rect;
	char		*str;
	t_point		mousepos;
	float		add;

	rect = empty_rect();
	add = 0.0f;
	if (gui_shoulddraw(gui)) 
	{
		str = ft_ftoa(*f, 4);
		rect = draw_text_boxed(gui->sdl, str, gui_currentpos(gui), gui->rect.size);
		drawrectangle(*gui->sdl, rect, CLR_BLUE);
		if (pointrectanglecollision(gui->hid->mouse.pos, rect))
		{
			draw_text_boxed(gui->sdl, "Drag to affect value", point_add(gui->hid->mouse.pos, (t_point){40, -10}), gui->sdl->screensize);
			drawrectangle(*gui->sdl, rect, CLR_GREEN);
			if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
			{
				mousepos = gui->hid->mouse.pos;
				force_mouselock(gui->hid);
				gui->hid->mouse.pos = mousepos;
				gui->hid->mouse.safe_delta = true;
				gui->player->locked = true;
				gui->locking_player = true;
			}
			if (gui->hid->mouse.relative && gui->locking_player)
			{
				add += (float)gui->hid->mouse.delta.x * mul;
			}
			if ((gui->hid->keystate >> KEYS_SHIFTMASK) & 1)
				add *= 2.0f;
			if ((gui->hid->keystate >> KEYS_LALTMASK) & 1)
				add *= 0.5f;
			*f += add;
		}
		free(str);
	}
	gui_layout(gui, rect);
}

void	gui_labeled_vector3_slider(char *str, t_vector3 *vec, float mul, t_autogui *gui)
{
	int	temp_min_x;

	temp_min_x = gui->min_x;
	
	gui_starthorizontal(gui);
	gui_label(str, gui);
	gui->min_x = 50;
	gui_float_slider(&vec->x, mul, gui);
	gui_float_slider(&vec->y, mul, gui);
	gui_float_slider(&vec->z, mul, gui);
	gui_endhorizontal(gui);
	gui->min_x = temp_min_x;
}

void	gui_vector3_slider(t_vector3 *vec, float mul, t_autogui *gui)
{
	int	temp_min_x;

	temp_min_x = gui->min_x;
	gui->min_x = 50;
	gui_starthorizontal(gui);
	gui_float_slider(&vec->x, mul, gui);
	gui_float_slider(&vec->y, mul, gui);
	gui_float_slider(&vec->z, mul, gui);
	gui_endhorizontal(gui);
	gui->min_x = temp_min_x;
}

static t_buttonreturn	autogui_internal_button(char *str, t_autogui *gui)
{
	t_buttonreturn	br;

	br.rect = draw_text_boxed(gui->sdl, str, gui_currentpos(gui), gui->sdl->screensize);
	drawrectangle(*gui->sdl, br.rect, CLR_BLUE);
	br.clicked = false;
	if (pointrectanglecollision(gui->hid->mouse.pos, br.rect))
	{
		drawrectangle(*gui->sdl, br.rect, CLR_GREEN);
		if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		{
			br.clicked = true;
			gui->hid->mouse.click_unhandled = false;
		}
	}
	return (br);
}

bool	gui_button(char *str, t_autogui *gui)
{
	t_rectangle		rect;
	t_buttonreturn	br;

	br.rect = empty_rect();
	br.clicked = false;
	if (gui_shoulddraw(gui))
	{
		br = autogui_internal_button(str, gui);
	}
	gui_layout(gui, br.rect);
	return (br.clicked);
}

void gui_int_slider(int *i, int mul, t_autogui *gui)
{
	char		*str;
	t_rectangle	rect;
	float		moveamount;
	t_point	potential_pos;

	rect = empty_rect();
	if (gui_shoulddraw(gui))
	{
		moveamount = gui->hid->mouse.scroll_delta * mul;
		if ((gui->hid->keystate >> KEYS_SHIFTMASK) & 1)
			moveamount *= 2;
		str = ft_itoa(*i);
		if (pointrectanglecollision(gui->hid->mouse.pos, rect))
		{
			draw_text_boxed(gui->sdl, "scroll to affect value", point_add(gui->hid->mouse.pos, (t_point){40, -10}), gui->sdl->screensize);
			drawrectangle(*gui->sdl, rect, CLR_BLUE);
			if (moveamount != 0)
				drawrectangle(*gui->sdl, rect, CLR_GREEN);
			*i = *i + moveamount;
		}
		free(str);
	}
	gui_layout(gui, rect);
}