/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_saveline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/04 09:25:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

void	saveline(t_editor *ed)
{
	t_list	*node;
	if (ed->linelist == NULL)
		ed->linelist = ft_lstnew(&ed->line, sizeof(t_line));
	else
	{
		node = ft_lstnew(&ed->line, sizeof(t_line));
		ft_lstapp(&ed->linelist, node);
	}
}