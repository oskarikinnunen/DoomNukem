/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 15:49:59 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/25 21:34:56 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor_tools.h"
#include "tools/guntool.h"
#include "doomnukem.h"
#include "objects.h"
#include "file_io.h"

void	gun_tool_cleanup(t_editor *ed, t_sdlcontext *sdl)
{
	ed->player.gun->disabled = true;
}

t_tool	*get_gun_tool(void)
{
	static t_tool	tool = {\
		.init = gun_tool_init, \
		.update = gun_tool_update, \
		.cleanup = gun_tool_cleanup};
	t_guntooldata	*dat;

	if (tool.tooldata == NULL)
		tool.tooldata = prot_memalloc(sizeof(t_guntooldata));
	ft_strncpy_term(tool.icon_name, "linetool.png", 250);
	return (&tool);
}
