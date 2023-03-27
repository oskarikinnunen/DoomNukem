/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_buttons2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:48:41 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 21:14:54 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"

char	*construct_shortcut_label(char *str, int alpha_or_keymask)
{
	char	*res;
	int		i;

	res = ft_strnew(ft_strlen(str) + 4);
	if (res == NULL)
		doomlog(LOG_EC_MALLOC, "gui_shortcut_button");
	if (ft_isalpha(alpha_or_keymask))
	{
		ft_strcpy(res, "[");
		i = ft_strlen(res);
		res[i] = alpha_or_keymask;
		res[i + 1] = '\0';
		ft_strcat(res, "]");
		ft_strcat(res, str);
	}
	else
	{
		ft_strcpy(res, "[");
		ft_strcat(res, str);
		ft_strcat(res, "]");
	}
	return (res);
}

bool	gui_shortcut_button(char *str, int alpha_or_keymask, t_autogui *gui)
{
	t_rectangle		rect;
	t_button_return	br;
	char			*str_s;

	br.rect = empty_rect();
	br.clicked = false;
	str_s = construct_shortcut_label(str, alpha_or_keymask);
	if (gui_should_draw(gui))
		br = autogui_internal_button(str_s, gui);
	free(str_s);
	gui_layout(gui, br.rect);
	if (alpha_or_keymask < 32 && (gui->hid->keystate >> alpha_or_keymask) & 1)
		br.clicked = true;
	if (ft_isalpha(alpha_or_keymask) && \
			check_alpha_key(gui->hid->alphakey_pressed, alpha_or_keymask))
		br.clicked = true;
	return (br.clicked);
}
