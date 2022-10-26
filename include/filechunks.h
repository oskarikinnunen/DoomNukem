/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filechunks.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:11:15 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/26 15:42:39 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILECHUNKS_H
# define FILECHUNKS_H 

# define CHUNKSIZE 4

typedef struct s_chunkloader
{
	char	chunkname[CHUNKSIZE + 1];
	size_t	size;
}	t_chunkloader;

#endif