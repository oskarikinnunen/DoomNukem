/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:56:41 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/28 14:18:28 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_IO_H
# define FILE_IO_H

# include "libft.h"

void	save_entitylist(t_list *entitylist, char *filename);
t_list	*load_chunk(char *filename, char *chunkname, size_t size);
void	save_chunk(char *filename, char *chunkname, t_list *content);
void	save_lists_to_file(t_editor *ed);

#endif