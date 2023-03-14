/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:56:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/14 12:58:19 by raho             ###   ########.fr       */
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

typedef enum e_load_arg
{
	LOAD_ARG_AMAP,
	LOAD_ARG_AMAP_AND_BASIC,
	LOAD_ARG_FULL
}	t_load_arg;

void	save_entitylist(t_list *entitylist, char *filename);
t_list	*load_chunk(char *filename, char *chunkname, size_t size);
void	save_chunk(char *filename, char *chunkname, t_list *content);


void			pack_file(char	*packname, char *filename);
void			force_pack_file(char *packname, char *filename);
t_filecontent	load_filecontent(char *worldname, char *fc_name);
//Loads a filecontent structure from a world file and writes it out to an actual file, returns an open fd
int				load_filecontent_fd(char *worldname, char *fcname);
void			load_and_write_filecontent(char *worldname, char *fcname, char *filename);


void	save_editordata(t_editor *ed);
int		fileopen(char *filename, int flags);
void	fileclose(int fd, char *filename);


void	generate_struct_datas();
void	load_struct(char	*filename, char *structname);
void	save_struct(char	filename[32], char *packname, char *structname, void *structdata);
/* World save/load */
char	*world_filename(char *worldname, char *suffix);
char	*extract_filename(const char *filepath);
int		count_asset_list(char *filename);



#endif