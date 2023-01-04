/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:56:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/03 16:34:31 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_IO_H
# define FILE_IO_H

# include "libft.h"
# include "editor_tools.h"

void	save_entitylist(t_list *entitylist, char *filename);
t_list	*load_chunk(char *filename, char *chunkname, size_t size);
void	save_chunk(char *filename, char *chunkname, t_list *content);
//void	save_filechunk(char	*filename, char *filechunkname, )
void	save_editordata(t_editor *ed);
int		fileopen(char *filename, int flags);

#endif