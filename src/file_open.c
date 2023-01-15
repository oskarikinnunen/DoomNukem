/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:16:48 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/09 17:55:08 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"

int	fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0666);
	if (fd == -1)
	{
		printf("FAILED TO OPEN FILE %s \n", filename);
		//error_log(EC_WRITE);
	}
		
	return (fd);
}