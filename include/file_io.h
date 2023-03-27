/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:56:41 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/26 21:05:55 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_IO_H
# define FILE_IO_H

# include "editor_tools.h"

# define CHUNKSIZE 4

typedef struct s_chunkloader
{
	char	chunk_name[CHUNKSIZE + 1];
	size_t	size;
}	t_chunkloader;

typedef struct s_filechunk
{
	char		file_name[64];
	uint32_t	file_length;
}	t_filechunk;

typedef struct s_file_content
{
	char		name[128];
	uint64_t	length;
	void		*content;
}	t_file_content;

typedef enum e_load_arg
{
	LOAD_ARG_AMAP,
	LOAD_ARG_AMAP_AND_BASIC,
	LOAD_ARG_FULL
}	t_load_arg;

void			save_entitylist(t_list *entitylist, char *file_name);
t_list			*load_chunk(char *file_name, char *chunk_name, size_t size);
int				read_chunk(int fd, t_file_content *fc, char *asset_name);
void			save_chunk(char *file_name, char *chunk_name, t_list *content);

// if offset is more than 0, filecontent seeks the information from 
// the offset till the end of file
t_file_content	load_filecontent(int fd, char *asset_name);
void			load_and_write_filecontent(int fd, char *fc_name, \
											char *file_name);
void			pack_file_to_level(char *level, char *file);

void			save_editor_data(t_editor *ed);
int				ft_fileopen(char *file_name, int flags);
int				ft_fileread(int fd, t_file_content *f);
void			ft_fileclose(int fd, char *file_name);
uint64_t		read_len(int fd);

void			load_struct(char *file_name, char *struct_name);
void			save_struct(char file_name[32], char *pack_name, \
							char *struct_name, void *struct_data);
/* World save/load */
void			world_save_amap(char *level, t_world world);
void			world_save_basic_ent(char *level, t_world world);
void			clean_create_level_file(char *level);
char			*extract_file_name(const char *file_path);
int				count_asset_list(char *file_name);

#endif