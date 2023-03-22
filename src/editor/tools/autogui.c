/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autogui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 16:19:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/21 14:56:49 by okinnune         ###   ########.fr       */
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
	gui.minimum_size.x = 240;
	gui.minimum_size.y = 128;
	return (gui);
}

static t_rectangle	empty_rect()
{
	return ((t_rectangle){0});
}

static void gui_limitrect(t_autogui *gui)
{
	gui->rect.size.x = ft_clamp(gui->rect.size.x, gui->minimum_size.x, 600);
	gui->rect.size.y = ft_clamp(gui->rect.size.y, gui->minimum_size.y, gui->sdl->screensize.y);
	gui->rect.size.x = ft_max(gui->rect.size.x, gui->x_maxdrawn);
	gui->rect.position.x = ft_clamp(gui->rect.position.x, 6, gui->sdl->window_w - 6 - gui->rect.size.x);
	gui->rect.position.y = ft_clamp(gui->rect.position.y, 6, gui->sdl->window_h - 6 - gui->rect.size.y);
}

void	gui_start(t_autogui *gui)
{
	if (gui->sdl == NULL)
		return ;
	if (!gui->hidden)
	{
		draw_rectangle_raster(*gui->sdl, gui->rect, 1);
		draw_rectangle(*gui->sdl, gui->rect, AMBER_0);
		t_rectangle	temp;
		temp.position = point_sub(gui->rect.position, point_one());
		temp.size = point_add(gui->rect.size, (t_point){2,2});
		draw_rectangle(*gui->sdl, temp, AMBER_1);
		temp.position = point_sub(temp.position, point_one());
		temp.size = point_add(temp.size, (t_point){2,2});
		draw_rectangle(*gui->sdl, temp, AMBER_2);
	}
	gui->overdraw = 0;
	gui->offset.y = 32 + 5; //TODO: autogui_start
	gui->offset.x = 5;
	if (gui->rect.size.x < gui->minimum_size.x)
		gui->rect.size.x = gui->minimum_size.x;
	if (gui->rect.size.y < gui->minimum_size.y)
		gui->rect.size.y = gui->minimum_size.y;
	gui->x_maxdrawn = 0;
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
	draw_rectangle(*gui->sdl, scrollrect, AMBER_0);
	if (gui->hid->mouse.held == MOUSE_LEFT && pointrectanglecollision(gui->hid->mouse.pos, scrollrect))
		gui->scroll_held = true;
	if (pointrectanglecollision(gui->hid->mouse.pos, scrollrect))
		draw_rectangle(*gui->sdl, scrollrect, AMBER_1);
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
				draw(*sdl, point_add(rect.position, (t_point){-p.x + rect.size.x, p.y}), clr);
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
	draw_rectangle_filled(*gui->sdl, hidecross, AMBER_1);
	draw_rectangle(*gui->sdl, hidecross, AMBER_0);
	p1 = hidecross.position;
	p2 = point_add(hidecross.position, hidecross.size);
	drawline(*gui->sdl, p1, p2, AMBER_3);
	p1 = point_add(hidecross.position, (t_point){.x = hidecross.size.x});
	p2 = point_add(hidecross.position, (t_point){.y = hidecross.size.y});
	drawline(*gui->sdl, p1, p2, AMBER_3);
	drawline(*gui->sdl, hidecross.position, point_add(hidecross.position, hidecross.size), AMBER_3);
	if (clicked_rectangle(gui->hid->mouse, hidecross))
		gui->hidden = true;
}

void	gui_autosize(t_autogui *gui)
{
	t_rectangle	titlerect;
	t_point		pos;

	pos = point_add(gui->rect.position, (t_point){5, 5});
	titlerect = print_text_boxed(gui->sdl, gui->title, pos);
	gui->minimum_size.x = titlerect.size.x + 32;
	gui->rect.size.x = titlerect.size.x + 32;
}

void	gui_end(t_autogui *gui)
{
	t_rectangle	dragcorner;
	t_rectangle	dragbar;

	if (gui->hidden || gui->sdl == NULL)
		return ;
	dragbar = gui->rect;
	dragbar.size.y = 32;
	draw_rectangle_filled(*gui->sdl, dragbar, 1);
	if (gui->dock != NULL)
	{
		gui->rect.position = point_add(gui->dock->rect.position, (t_point){.x = gui->dock->rect.size.x + 2});
		draw_rectangle(*gui->sdl, dragbar, CLR_DARKGRAY);
	}
	else if (!gui->locked)
	{
		draw_rectangle(*gui->sdl, dragbar, AMBER_0);
		if (pointrectanglecollision(gui->hid->mouse.pos, dragbar) || gui->move_held)
			draw_rectangle(*gui->sdl, dragbar, AMBER_1);
		if (gui->hid->mouse.held == MOUSE_LEFT && pointrectanglecollision(gui->hid->mouse.pos, dragbar) && !gui->hid->mouse.dragging_ui)
		{
			//gui->hid->mouse.dragging_ui = true;
			gui->move_held = true;
		}
			
		if (gui->move_held && !gui->locked)
			gui->rect.position = point_sub(gui->hid->mouse.pos, point_div(dragbar.size, 2));
	}
	if (gui->allow_user_hide)
	{
		update_hidecross(gui);
	}
	if (!gui->locked)
	{
		dragcorner.size = (t_point) {16,16};
		dragcorner.position = point_sub(point_add(gui->rect.position, gui->rect.size), dragcorner.size);
		draw_rect_tri(gui->sdl, dragcorner, AMBER_0);
		if (pointrectanglecollision(gui->hid->mouse.pos, dragcorner) || gui->drag_held)
			draw_rect_tri(gui->sdl, dragcorner, AMBER_1);
		if (gui->hid->mouse.held == MOUSE_LEFT && pointrectanglecollision(gui->hid->mouse.pos, dragcorner) && !gui->hid->mouse.dragging_ui)
		{
			//gui->hid->mouse.dragging_ui = true;
			gui->drag_held = true;
		}
		if (gui->drag_held)
		{
			gui->scroll.y = 0;
			gui->rect.size = point_sub(point_add(gui->hid->mouse.pos, point_div(dragcorner.size, 2)), gui->rect.position);
		}
	}
	if (gui->hid->mouse.held == 0)
	{
		gui->move_held = false;
		gui->drag_held = false;
		if (gui->hid->mouse.dragging_ui)
		{
			gui->hid->mouse.dragging_ui = false;
			force_mouseunlock(gui->hid);
		}
		gui->scroll_held = false;
	}
	if (gui->offset.y < gui->rect.size.y)
	{
		gui->scroll.y = 0;
		gui->scrollable = false;
		//gui->minimum_size.y = gui->offset.y + 32;
		gui->rect.size.y = gui->offset.y + 32;
	}
	if (gui->scrollable)
	{
		gui->scroll.y += gui->hid->mouse.scroll_delta * 16;
		update_scrollbar(gui);
		gui->scroll.y = ft_clamp(gui->scroll.y, -gui->overdraw, 0);
	}
	gui_limitrect(gui);
	//set_font_size(gui->sdl, 2);
	//char temp[128];
	//sprintf(temp, "size: %i %i min: %i %i\n", gui->rect.size.x, gui->rect.size.y, gui->minimum_size.x, gui->minimum_size.y);
	//print_text(gui->sdl, temp, gui->rect.position);
	print_text_colored(gui->sdl, gui->title, point_add(gui->rect.position, (t_point){5, 5}), AMBER_3);
	//set_font_size(gui->sdl, 0);
}

static t_point gui_currentpos(t_autogui *gui)
{
	t_point	pos;

	pos = point_add(gui->rect.position, point_add(gui->scroll, gui->offset));
	pos = point_add(pos, (t_point){5,0});
	return (pos);
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

	x = ft_max(rect.size.x + 10, gui->min_x);
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
	if (gui->offset.x > gui->x_maxdrawn)
		gui->x_maxdrawn = gui->offset.x;
}

static	void	gui_layout_big(t_autogui *gui, t_rectangle rect)
{
	int	x;

	x = ft_max(rect.size.x + 10, gui->min_x);
	if (gui->agl == agl_vertical)
	{
		if (!gui_shoulddraw(gui))
		{
			gui->overdraw += 34;
		}
		gui->offset.y += 34;
	}
	else
		gui->offset.x += x;
	if (gui->offset.x > gui->x_maxdrawn)
		gui->x_maxdrawn = gui->offset.x;
}

void	gui_emptyvertical(int y, t_autogui *gui)
{
	t_rectangle	rect;

	rect.size.x = 0;
	rect.size.y = y;
	gui_layout(gui, rect);
}

bool	gui_labeled_float_slider(char *str, float *f, float mul, t_autogui *gui)
{
	bool	modified;

	modified = false;
	gui_starthorizontal(gui);
	gui_label(str, gui);
	modified = gui_float_slider(f, mul, gui);
	gui_endhorizontal(gui);
	return (modified);
}

void	gui_label(char *str, t_autogui *gui)
{
	t_rectangle	rect;

	rect = empty_rect();
	if (gui_shoulddraw(gui)) 
	{
		rect = print_text_colored(gui->sdl, str, gui_currentpos(gui), AMBER_3);
	}
	gui_layout(gui, rect);
}

void	gui_preset_scale_and_rotate(t_transform *t, t_autogui *gui)
{
	bool	modified;

	gui_labeled_vector3_slider("Rotation:", &t->rotation, 0.1f, gui);
	gui_labeled_vector3_slider("Scale:", &t->scale, 0.1f, gui);
	modified = gui_labeled_float_slider("Scale (Locked xyz):", &t->scale.z, 0.1f, gui);
	if (modified)
	{
		t->scale.x = t->scale.z;
		t->scale.y = t->scale.z;
	}
}

void	gui_preset_transform(t_transform *t, t_autogui *gui)
{
	bool	modified;

	gui_labeled_vector3_slider("Position:", &t->position, 1.0f, gui);
	gui_labeled_vector3_slider("Rotation:", &t->rotation, 0.1f, gui);
	gui_labeled_vector3_slider("Scale:", &t->scale, 0.1f, gui);
	modified = gui_labeled_float_slider("Scale (Locked xyz):", &t->scale.z, 0.1f, gui);
	if (modified)
	{
		t->scale.x = t->scale.z;
		t->scale.y = t->scale.z;
	}
}

bool	gui_float_slider(float	*f, float mul, t_autogui *gui)
{
	t_rectangle	rect;
	char		*str;
	t_point		mousepos;
	float		add;
	bool		modified;

	rect = empty_rect();
	add = 0.0f;
	modified = false;
	if (gui_shoulddraw(gui)) 
	{
		str = ft_ftoa(*f, 4);
		rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
		draw_rectangle(*gui->sdl, rect, AMBER_0);
		if (pointrectanglecollision(gui->hid->mouse.pos, rect))
		{
			print_text_boxed(gui->sdl, "Drag to affect value", point_add(gui->hid->mouse.pos, (t_point){40, -10}));
			draw_rectangle(*gui->sdl, rect, AMBER_1);
			if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
			{
				mousepos = gui->hid->mouse.pos;
				force_mouselock(gui->hid);
				gui->hid->mouse.pos = mousepos;
				gui->hid->mouse.dragging_ui = true;
			}
			if (gui->hid->mouse.held == MOUSE_LEFT)
			{
				add += (float)gui->hid->mouse.delta.x * mul;
				if (add != 0.0f)
					modified = true;
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
	return (modified);
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

static t_buttonreturn	autogui_internal_colored_button(char *str, t_autogui *gui, uint32_t color)
{
	t_buttonreturn	br;

	br.rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
	draw_rectangle(*gui->sdl, br.rect, AMBER_2);
	br.clicked = false;
	if (pointrectanglecollision(gui->hid->mouse.pos, br.rect))
	{
		draw_rectangle(*gui->sdl, br.rect, color);
		if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		{
			br.clicked = true;
			gui->hid->mouse.click_unhandled = false;
		}
	}
	return (br);
}

static t_buttonreturn	autogui_internal_button(char *str, t_autogui *gui)
{
	t_buttonreturn	br;

	br.rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
	draw_rectangle(*gui->sdl, br.rect, AMBER_0);
	br.clicked = false;
	if (pointrectanglecollision(gui->hid->mouse.pos, br.rect))
	{
		draw_rectangle(*gui->sdl, br.rect, AMBER_2);
		if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		{
			br.clicked = true;
			gui->hid->mouse.click_unhandled = false;
		}
	}
	return (br);
}

bool	gui_colored_button(char *str, t_autogui *gui, uint32_t color)
{
	t_rectangle		rect;
	t_buttonreturn	br;

	br.rect = empty_rect();
	br.clicked = false;
	if (gui_shoulddraw(gui))
	{
		br = autogui_internal_colored_button(str, gui, color);
	}
	gui_layout(gui, br.rect);
	return (br.clicked);
}

bool	gui_shortcut_button(char *str, int alpha_or_keymask, t_autogui *gui)
{
	t_rectangle		rect;
	t_buttonreturn	br;
	char			*str_s;

	br.rect = empty_rect();
	br.clicked = false;
	str_s = ft_strnew(ft_strlen(str) + 4);
	if (ft_isalpha(alpha_or_keymask))
		snprintf(str_s, ft_strlen(str) + 4, "[%c]%s", alpha_or_keymask, str);
	else
		snprintf(str_s, ft_strlen(str) + 4, "[%s]", str);
	if (gui_shoulddraw(gui))
	{
		br = autogui_internal_button(str_s, gui);
	}
	free(str_s);
	gui_layout(gui, br.rect);
	if (alpha_or_keymask < 32 && (gui->hid->keystate >> alpha_or_keymask) & 1) //Assumed keymask
		br.clicked = true;
	if (ft_isalpha(alpha_or_keymask) && check_alpha_key(gui->hid->alphakey_pressed, alpha_or_keymask))
		br.clicked = true;
	return (br.clicked);
}

bool	gui_imagebutton(t_img	*img, t_autogui *gui)
{
	t_rectangle		imgrect;
	bool			ret;

	imgrect = empty_rect();
	ret = false;
	if (gui_shoulddraw(gui))
	{
		draw_image(*gui->sdl, gui_currentpos(gui), *img, (t_point){32, 32});
		imgrect.size = (t_point){32, 32};
		imgrect.position = gui_currentpos(gui);
		if (pointrectanglecollision(gui->hid->mouse.pos, imgrect))
		{
			print_text(gui->sdl, img->name, gui->hid->mouse.pos);
			if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
				ret = true;
		}
	}
	gui_layout_big(gui, imgrect);
	return (ret);
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

bool	gui_hoverlabel(char *str, t_autogui *gui)
{
	t_rectangle	rect;
	bool		hovered;

	rect = empty_rect();
	hovered = false;
	if (gui_shoulddraw(gui))
	{
		rect = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
		if (pointrectanglecollision(gui->hid->mouse.pos, rect))
			hovered = true;
	}
	gui_layout(gui, rect);
	return (hovered);
}

//TODO: big norminette thing
void	gui_string_edit(char *str, t_autogui	*gui)
{

	SDL_Event	e;
	bool		done;
	t_rectangle	rect;
	t_rectangle	orig;
	t_point		start;
	char		bstr[40];

	ft_strcpy(bstr, str);
	done = false;
	orig = print_text_boxed(gui->sdl, str, gui_currentpos(gui));
	rect = empty_rect();
	if (gui_colored_button(bstr, gui, AMBER_4))
	{
		SDL_StartTextInput();
		while (!done)
		{
			while (SDL_PollEvent(&e) && !done)
			{
				draw_rectangle_filled(*gui->sdl, rect, 1);
				if (e.type == SDL_TEXTINPUT && ft_strlen(str) < 31)
				{
					ft_strncat(str, e.text.text, 32);
				}
				if (e.type == SDL_KEYDOWN && iskey(e, SDLK_BACKSPACE))
					str[ft_strlen(str) - 1] = '\0';
				rect = empty_rect();
				ft_bzero(bstr, 40);
				sprintf(bstr, "\xE6 %s ", str);
				rect = print_text_boxed(gui->sdl, bstr, orig.position);
				ft_memcpy(gui->sdl->window_surface->pixels, gui->sdl->surface->pixels, gui->sdl->window_w * gui->sdl->window_h * sizeof(uint32_t));
				SDL_UpdateWindowSurface(gui->sdl->window);
				if (e.type == SDL_MOUSEBUTTONDOWN
					|| (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RETURN ||
											e.key.keysym.sym == SDLK_RETURN2 ||
											e.key.keysym.sym == SDLK_ESCAPE)))
				{
					done = true;
					break ;
				}
			}
		}
		SDL_StopTextInput();
	}
}

bool	gui_highlighted_button(char *str, t_autogui *gui)
{
	t_rectangle		rect;
	t_buttonreturn	br;

	br.rect = empty_rect();
	br.clicked = false;
	if (gui_shoulddraw(gui))
	{
		br = autogui_internal_button(str, gui);
	}
	draw_rectangle(*gui->sdl, br.rect, AMBER_3);
	gui_layout(gui, br.rect);
	return (br.clicked);
}

bool	gui_highlighted_button_if(char *str, t_autogui *gui, bool condition)
{
	if (condition)
		return (gui_highlighted_button(str, gui));
	else
		return (gui_button(str, gui));
}

bool	gui_labeled_bool(char *str, bool b, t_autogui *gui)
{
	gui_starthorizontal(gui);
	gui_label(str, gui);
	if (b)
		gui_label("True", gui);
	else
		gui_label("False", gui);
	gui_endhorizontal(gui);
}

//TODO: 'modified' is always false
bool	gui_labeled_bool_edit(char *str, bool *b, t_autogui *gui)
{
	bool	modified;

	modified = false;
	gui_starthorizontal(gui);
	gui_label(str, gui);
	if (gui_highlighted_button_if("True", gui, *b))
		*b = true;
	if (gui_highlighted_button_if("False", gui, !*b))
		*b = false;
	gui_endhorizontal(gui);
	return (modified);
}

bool	gui_bool_edit(bool *b, t_autogui *gui)
{
	gui_starthorizontal(gui);
	if (*b)
	{
		gui_highlighted_button("True", gui);
		if (gui_button("False", gui))
			*b = false;
	}
	else
	{
		if (gui_button("True", gui))
			*b = true;
		gui_highlighted_button("False", gui);
	}
	gui_endhorizontal(gui);
}

void	gui_int(int i, t_autogui *gui)
{
	char		*str;
	t_rectangle	rect;

	rect = empty_rect();
	if (gui_shoulddraw(gui))
	{
		str = ft_itoa(i);
		rect = print_text_colored(gui->sdl, str, gui_currentpos(gui), AMBER_3);
		free(str);
	}
	gui_layout(gui, rect);
}

void	gui_point(t_point point, t_autogui *gui)
{
	gui_starthorizontal(gui);
	gui_int(point.x, gui);
	gui_int(point.y, gui);
	gui_endhorizontal(gui);
}

void	gui_labeled_point(char *str, t_point point, t_autogui *gui)
{
	gui_starthorizontal(gui);
	gui_label(str, gui);
	gui_int(point.x, gui);
	gui_int(point.y, gui);
	gui_endhorizontal(gui);
}

void	gui_labeled_int(char *str, int i, t_autogui *gui)
{
	gui_starthorizontal(gui);
	gui_label(str, gui);
	gui_int(i, gui);
	gui_endhorizontal(gui);
}

void	gui_lock_mouse(t_autogui *gui)
{
	t_point	mousepos;

	mousepos = gui->hid->mouse.pos;
	force_mouselock(gui->hid);
	gui->hid->mouse.pos = mousepos;
	gui->hid->mouse.dragging_ui = true;
}

bool	gui_int_slider_internal(int *i, float mul,
								t_rectangle rect, t_autogui *gui)
{
	int		add;
	bool	modified;

	add = 0;
	modified = 0;
	print_text_boxed(gui->sdl, "Drag to affect value",
		point_add(gui->hid->mouse.pos, (t_point){40, -10}));
	draw_rectangle(*gui->sdl, rect, AMBER_1);
	if (mouse_clicked(gui->hid->mouse, MOUSE_LEFT))
		gui_lock_mouse(gui);
	if (gui->hid->mouse.held == MOUSE_LEFT)
	{
		add = (float)gui->hid->mouse.delta.x;
		modified = (add != 0);
	}
	*i = *i + add;
	return (modified);
}

bool	gui_int_slider(int *i, float mul, t_autogui *gui)
{
	t_rectangle	rect;
	char		*str;
	t_point		mousepos;
	int			add;
	bool		modified;

	rect = empty_rect();
	add = 0;
	modified = false;
	if (gui_shoulddraw(gui))
	{
		rect = print_text_boxed(gui->sdl, s_itoa(*i), gui_currentpos(gui));
		draw_rectangle(*gui->sdl, rect, AMBER_0);
		if (pointrectanglecollision(gui->hid->mouse.pos, rect))
			modified = gui_int_slider_internal(i, mul, rect, gui);
	}
	gui_layout(gui, rect);
	return (modified);
}

bool	gui_labeled_int_slider(char *str, int *i, float mul, t_autogui *gui)
{
	bool	modified;

	modified = false;
	gui_starthorizontal(gui);
	gui_label(str, gui);
	modified = gui_int_slider(i, mul, gui);
	gui_endhorizontal(gui);
	return (modified);
}
