/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:56:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/09 16:31:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_IO_H
# define FILE_IO_H

# include "libft.h"
# include "editor_tools.h"

typedef struct s_filecontent
{
	char		name[128];
	uint64_t	length;
	void		*content;
}	t_filecontent;

void	save_entitylist(t_list *entitylist, char *filename);
t_list	*load_chunk(char *filename, char *chunkname, size_t size);
void	save_chunk(char *filename, char *chunkname, t_list *content);


void			save_filecontent(char	*worldname, char *filename);
t_filecontent	load_filecontent(char	*worldname, char	*fc_name);
//Loads a filecontent structure from a world file and writes it out to an actual file, returns an open fd
int				load_filecontent_fd(char	*worldname, char *fcname);


void	save_editordata(t_editor *ed);
int		fileopen(char *filename, int flags);

#endif