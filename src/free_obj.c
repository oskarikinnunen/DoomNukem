/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:16:35 by vlaine            #+#    #+#             */
/*   Updated: 2022/10/05 18:20:58 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

static	void	free_faces(t_obj *obj)
{
	int	len;
	int	i;

	i = 0;
	if (!obj->faces)
		return ;
	len = GRIDSIZE * GRIDSIZE;
	obj->f_count = (len - GRIDSIZE - (len - GRIDSIZE) / GRIDSIZE) * 2; //FACECOUNT, not tricount?
	obj->f_count -= GRIDSIZE;
	while (i < obj->f_count)
	{
		if (obj->faces[i])
			free(obj->faces[i]);
		if (obj->faces[i + 1])
			free(obj->faces[i + 1]);
		i += 2;
	}
	free(obj->faces);
}

void	free_obj(t_obj *obj)
{
	return ;
	int xy[2];

	obj->v_count = 0;
	if (!obj->verts)
		return ;
	xy[Y] = 0;
	while (xy[Y] < TILESIZE * GRIDSIZE)
	{
		xy[X] = 0;
		while (xy[X] < TILESIZE * GRIDSIZE)
		{
			if (obj->verts[obj->v_count])
				free(obj->verts[obj->v_count]);
			obj->v_count++;
			xy[X] += TILESIZE;
		}
		xy[Y] += TILESIZE;
	}
	free(obj->verts);
	//free_faces(obj);
}

void	free_lst(t_list *head)
{
	t_list *temp;

	while (head)
	{
		temp = head;
		head = (head)->next;
		if (temp->content)
			free(temp->content);
		free(temp);
	}
}
