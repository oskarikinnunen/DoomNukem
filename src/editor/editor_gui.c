/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:09:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/25 16:19:54 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"

t_point	*get_resolutions(void)
{
	static t_point	resolutions[32] = \
	{\
		{640, 480}, \
		{800, 600}, \
		{1024, 768}, \
		{960, 540}, \
		{1280, 720}, \
		{1366, 768}, \
		{1600, 900}, \
		{1920, 1080}, \
		{2560, 1440} \
	};

	return (resolutions);
}

char	*resolution_str(t_point res)
{
	char	*str1;
	char	*str2;
	char	*final;

	str1 = ft_itoa(res.x);
	if (str1 == NULL)
		doomlog(LOG_EC_MALLOC, "resolution_str");
	str2 = ft_itoa(res.y);
	if (str2 == NULL)
		doomlog(LOG_EC_MALLOC, "resolution_str");
	final = ft_strnew(ft_strlen(str1) + ft_strlen(str2) + 1);
	if (final == NULL)
		doomlog(LOG_EC_MALLOC, "resolution_str");
	ft_strcat(final, str1);
	ft_strcat(final, "x");
	ft_strcat(final, str2);
	free(str1);
	free(str2);
	return (final);
}

void	screenmode_toggles(t_autogui *gui, t_graphicprefs *prefs)
{
	if (prefs->screenmode == screenmode_fullscreen)
	{
		if (gui_highlighted_button("Fullscreen", gui))
			prefs->screenmode = screenmode_fullscreen;
		if (gui_button("Windowed", gui))
			prefs->screenmode = screenmode_windowed;
	}
	else
	{
		if (gui_button("Fullscreen", gui))
			prefs->screenmode = screenmode_fullscreen;
		if (gui_highlighted_button("Windowed", gui))
			prefs->screenmode = screenmode_windowed;
	}
}

//TODO: name better
static void	update_editor_lategui_helper(t_editor_late_gui *elg, t_autogui *gui)
{
	gui_labeled_float_slider(\
			"Audio volume:", &gui->sdl->audio.sfx_volume, 0.01f, gui);
	gui->sdl->audio.sfx_volume = \
			ft_clampf(gui->sdl->audio.sfx_volume, 1.0f, 10.0f);
	if (gui_labeled_float_slider(\
			"Music volume:", &gui->sdl->audio.music_volume, 0.01f, gui))
	{
		gui->sdl->audio.music_volume = \
				ft_clampf(gui->sdl->audio.music_volume, 0.0f, 1.0f);
		FMOD_Channel_SetVolume(\
				gui->sdl->audio.music_channel, gui->sdl->audio.music_volume);
	}
	gui_labeled_float_slider(\
			"Resolution scale:", &gui->sdl->resolution_scaling, 0.01f, gui);
	gui->sdl->resolution_scaling = \
			ft_clampf(gui->sdl->resolution_scaling, 0.25f, 1.0f);
	elg->prefs.resolutionscale = gui->sdl->resolution_scaling;
	elg->res = point_fmul(gui->sdl->screensize, gui->sdl->resolution_scaling);
	gui_labeled_point("3D Resolution:", elg->res, gui);
	gui_labeled_bool_edit("Blend:", &gui->sdl->blend, gui);
	screenmode_toggles(gui, &elg->prefs);
	gui_end(gui);
}

//TODO: make this graphics_settings_gui
void	update_editor_lateguis(t_editor *ed)
{
	t_editor_late_gui	elg;
	t_autogui			*gui;

	gui = &ed->graphics_gui;
	elg.resolutions = get_resolutions();
	gui_start(gui);
	gui_label("Select resolution:", gui);
	if (elg.prefs.resolution_x == 0)
		elg.prefs = get_prefs_from_sdl(gui->sdl);
	while (elg.i < 10)
	{
		elg.str = resolution_str(elg.resolutions[elg.i]);
		if ((point_cmp(ed->world.sdl->screensize, elg.resolutions[elg.i]) && \
				gui_highlighted_button(elg.str, gui)) || \
				(!point_cmp(ed->world.sdl->screensize, \
				elg.resolutions[elg.i]) && gui_button(elg.str, gui)))
		{
			elg.prefs.resolution_x = elg.resolutions[elg.i].x;
			elg.prefs.resolution_y = elg.resolutions[elg.i].y;
			apply_graphics_prefs(elg.prefs);
		}
		free(elg.str);
		elg.i++;
	}
	update_editor_lategui_helper(&elg, gui);
}
