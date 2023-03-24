/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_gui.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:09:55 by raho              #+#    #+#             */
/*   Updated: 2023/03/24 22:15:15 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
#include "editor_tools.h"


t_point	*get_resolutions()
{
	static t_point resolutions[32] =
	{
		{320, 240},
		{640, 480},
		{800, 600},
		{1024, 768},
		{960, 540},
		{1280, 720},
		{1366, 768},
		{1600, 900},
		{1920, 1080},
		{2560, 1440}
	};

	return (resolutions);
}

char	*resolution_str(t_point res)
{
	char	*str1;
	char	*str2;
	char	*final;

	str1 = ft_itoa(res.x);
	str2 = ft_itoa(res.y);

	final = ft_strnew(ft_strlen(str1) + ft_strlen(str2) + 1);
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

void	update_editor_lateguis(t_editor *ed) //TODO: make this graphics_settings_gui
{
	t_autogui				*gui;
	static	t_graphicprefs	prefs;
	t_point					*resolutions;
	int						i;
	char					*str;
	
	gui = &ed->graphics_gui;
	resolutions = get_resolutions();

	gui_start(gui);
	gui_label("Select resolution:", gui);
	if (prefs.resolution_x == 0)
		prefs = get_prefs_from_sdl(gui->sdl);
	i = 0;
	while (i < 10)
	{
		str = resolution_str(resolutions[i]);
		if ((point_cmp(ed->world.sdl->screensize, resolutions[i]) && gui_highlighted_button(str, gui))
			|| (!point_cmp(ed->world.sdl->screensize, resolutions[i]) && gui_button(str, gui)))
		{
			prefs.resolution_x = resolutions[i].x;
			prefs.resolution_y = resolutions[i].y;
			apply_graphics_prefs(prefs);
		}
		free(str);
		i++;
	}
	gui_labeled_float_slider("Audio volume:", &gui->sdl->audio.sfx_volume, 0.01f, gui);
	gui->sdl->audio.sfx_volume = ft_clampf(gui->sdl->audio.sfx_volume, 1.0f, 10.0f);
	if (gui_labeled_float_slider("Music volume:", &gui->sdl->audio.music_volume, 0.01f, gui))
	{
		gui->sdl->audio.music_volume = ft_clampf(gui->sdl->audio.music_volume, 0.0f, 1.0f);
		FMOD_Channel_SetVolume(gui->sdl->audio.music_channel, gui->sdl->audio.music_volume);
	}
	
	gui_labeled_float_slider("Resolution scale:", &gui->sdl->resolution_scaling, 0.01f, gui);
	gui->sdl->resolution_scaling = ft_clampf(gui->sdl->resolution_scaling, 0.25f, 1.0f);
	prefs.resolutionscale = gui->sdl->resolution_scaling;
	t_point	res;
	res = point_fmul(gui->sdl->screensize, gui->sdl->resolution_scaling);
	gui_labeled_point("3D Resolution:", res, gui);
	gui_labeled_bool_edit("Blend:", &gui->sdl->blend, gui);
	if (gui_button("Apply settings", gui))
	{
		apply_graphics_prefs(prefs);
	}
	screenmode_toggles(gui, &prefs);
	gui_end(gui);
}