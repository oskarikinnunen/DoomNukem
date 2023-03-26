/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:57:03 by raho              #+#    #+#             */
/*   Updated: 2023/03/26 20:18:00 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

//TODO: RENAME
void	multi_thread_start(t_thread *thread)
{
	int			i;
	void		*ptr;

	i = 0;
	while (i < thread->count)
	{
		ptr = &(thread->structs[i * thread->struct_size]);
		pthread_create(&thread->threads[i], NULL, thread->func, ptr);
		i++;
	}
	i = 0;
	while (i < thread->count)
	{
		pthread_join(thread->threads[i], NULL);
		i++;
	}
	ft_bzero(thread->structs, thread->struct_size * THREAD);
	thread->count = 0;
}
