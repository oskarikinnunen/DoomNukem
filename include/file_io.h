/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:56:41 by okinnune          #+#    #+#             */
/*   Updated: 2022/11/21 19:23:35 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_IO_H
# define FILE_IO_H

# include "libft.h"

void	save_entitylist(t_list *entitylist, char *filename);
t_list	*load_chunk(char *filename, char *chunkname, size_t size);
void	save_chunk(char *filename, char *chunkname, t_list *content);
void	save_editordata(t_editor *ed);
int		fileopen(char *filename, int flags);

#endif