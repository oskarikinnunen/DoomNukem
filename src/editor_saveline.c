/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_saveline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/20 14:16:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"


void	saveline(t_editor *ed)
{
	t_list	*node;
	
	if (ed->linelist == NULL)
	{
		ed->linelist = ft_lstnew(&ed->line, sizeof(t_line));
		if (!ed->linelist)
			error_log(EC_MALLOC);
	}
	else
	{
		node = ft_lstnew(&ed->line, sizeof(t_line));
		if (!node)
			error_log(EC_MALLOC);
		ft_lstapp(&ed->linelist, node);
	}
}
