/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structsaver.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 22:11:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/17 11:29:14 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "libft.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <doomnukem.h>
#include <file_io.h>


//TODO:
/*
	enums,
	different int sizes
*/
typedef enum e_valuetype
{
	value_float,
	value_int,
	value_struct,
	value_missing,
	value_ptr
}	t_valuetype;

typedef struct s_valuemetadata
{
	char		name[32];
	char		typename[32];
	t_valuetype	type;
	size_t		size;
	size_t		mem_offset;
}	t_valuemetadata;

typedef struct s_structmetadata
{
	char			name[32];
	t_list			*values;
}	t_structmetadata;

bool	string_contains(char *haystack, char *needle)
{
	return (ft_strstr(haystack, needle) != NULL);
}

bool	string_is_match(char *string1, char *string2)
{
	return (ft_strcmp(string1, string2) == 0);
}

typedef struct s_structanalyzer
{
	bool				reading;
	t_structmetadata		cur_struct;
	t_list				*structs;
}	t_structanalyzer;

bool	struct_ends(char *line)
{
	return (string_contains(line, "}") && string_contains(line, ";"));
}

bool	structmetadata_has_missing_values(t_structmetadata *smd)
{
	t_list				*l_val;
	t_valuemetadata		*val;

	l_val = smd->values;
	while (l_val != NULL)
	{
		val = (t_valuemetadata *)l_val->content;
		if (val->type == value_missing)
			return (true);
		l_val = l_val->next;
	}
	return (false);
}

void	set_structname(t_structmetadata *saved, char *line)
{
	char	*a;
	int		i;

	i = 0;
	while (line[i] == '}' || line[i] == ' ' || line[i] == '\t')
		i++;
	
	ft_strcpy(saved->name, &line[i]);
	saved->name[ft_strlen(saved->name) - 1] = 0;
}

int	splitlen(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		i++;
	}
	return (i);
}

void	try_parse_value(char *line, t_structmetadata *saved)
{
	t_valuemetadata	value;
	char			**split;
	int				i;

	ft_bzero(&value, sizeof(value));
	split = ft_strscrape(line, "\t ");
	if (splitlen(split) == 2)
	{
		if (string_is_match(split[0], "float"))
		{
			value.size = 4UL;
			value.type = value_float;
		}
		else if (string_contains(split[0], "int") || string_contains(split[0], "bool"))
		{
			value.size = 4UL;
			value.type = value_int;
		}
		else if (string_contains(split[1], "*"))
		{
			value.size = 8UL;
			value.type = value_ptr;
		}
		else
			value.type = value_missing;
		ft_strcpy(value.typename, split[0]);
		ft_strcpy(value.name, split[1]);
		value.name[ft_strlen(value.name) - 1] = '\0';
		//printf("	variable %s\n		bits: %lu missing: %i ptr: %i\n", value.name, value.size * 8, value.type == value_missing, value.type == value_ptr);
		list_push(&saved->values, &value, sizeof(value));
	}
}

void	print_missing_types(t_list	*savedatas)
{
	t_list				*l_sd;
	t_list				*l_val;
	t_structmetadata		*sd;
	t_valuemetadata		*val;
	int					i;

	l_sd = savedatas;
	i = 0;
	while (l_sd != NULL) 
	{
		sd = (t_structmetadata *)l_sd->content;
		l_val = sd->values;
		i = 0;
		while (l_val != NULL)
		{
			val = (t_valuemetadata *)l_val->content;
			if (val->type == value_missing)
				i++;
			l_val = l_val->next;
		}
		printf("%s has %i missing types \n", sd->name, i);
		l_sd = l_sd->next;
	}
}

size_t	sizeof_struct(t_structmetadata *saved)
{
	t_list				*l_val;
	t_valuemetadata		*val;
	size_t				size;

	l_val = saved->values;
	size = 0;
	while (l_val != NULL)
	{
		val = (t_valuemetadata *)l_val->content;
		size += val->size;
		l_val = l_val->next;
	}
	return (size);
}

void	fix_missing_types(t_structanalyzer	*saver, t_structmetadata	*saved)
{
	t_list				*l_sd;
	t_list				*l_val;
	t_valuemetadata		*val;
	t_structmetadata		*sd;

	l_sd = saver->structs;
	while (l_sd != NULL) 
	{
		sd = (t_structmetadata *)l_sd->content;
		if (sd != saved)
		{
			//printf("checking struct %s for match \n", sd->name);
			l_val = saved->values;
			while (l_val != NULL)
			{
				val = (t_valuemetadata *)l_val->content;
				//printf("checking if '%s %s'  matches '%s'\n", val->typename, val->name, sd->name);
				if (string_is_match(val->typename, sd->name))
				{
					val->size = sizeof_struct(sd);
					val->type = value_struct;
					//printf("found missing value type %s as struct of size %lu \n", val->name, val->size);
				}
				l_val = l_val->next;
			}
		}
		/*if(string_is_match(sd->name, saved))
			return (sd);*/
		l_sd = l_sd->next;
	}
}

t_structmetadata	*find_struct_match(char *structname, t_structanalyzer *saver)
{
	t_list				*l_sd;
	t_structmetadata		*sd;

	l_sd = saver->structs;
	while (l_sd != NULL) 
	{
		sd = (t_structmetadata *)l_sd->content;
		if(string_is_match(sd->name, structname))
			return (sd);
		l_sd = l_sd->next;
	}
	return (NULL);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

static void	force_save_chunk(char *filename, char *chunkname, t_list *content)
{
	t_list		*l;
	int			written;
	int			fd;

	l = content;
	fd = fileopen(filename, O_RDWR | O_APPEND);
	if (fd == -1)
		fd = fileopen(filename, O_CREAT | O_RDWR | O_APPEND);
	written = 0;
	write(fd, chunkname, 4);
	while (l != NULL)
	{
		written += write(fd, l->content, l->content_size);
		l = l->next;
	}
	if (written % 4 != 0) //No need for padding since struct size 'should be' always multiple of 4? (Compiler does padding for structs) TODO: research more, will this happen when compiling for other platforms?
	{
		//printf("pad size = %i \n", CHUNKSIZE - (written % CHUNKSIZE)); //TODO: don't remove, going to be used for logging
		write(fd, "PADD", 4 - (written % 4));
	}
	write(fd, "CEND", 4);
	close(fd);
}
static void fancysave(char *filename, t_list *values)
{
	int				written;
	int				fd;
	t_list			*l;
	t_valuemetadata	*val;

	fd = fileopen(filename, O_RDWR | O_APPEND);
	if (fd == -1)
		fd = fileopen(filename, O_CREAT | O_RDWR | O_APPEND);
	l = values;
	while (l != NULL)
	{
		val = (t_valuemetadata *)l->content;
		write(fd, val->typename, ft_strlen(val->typename));
		write(fd, "\n\t", 2);
		write(fd, val->name, ft_strlen(val->name));
		//write(fd, &val->size, sizeof(size_t));
		write(fd, "\n", 1);
		//write(val->name
		l = l->next;
	}
	close(fd);
}

#pragma GCC diagnostic pop

void	print_memorychunks(t_structmetadata *stru)
{
	t_list			*l_val;
	t_valuemetadata	*val;
	int				i;

	l_val = stru->values;
	while (l_val != NULL) 
	{
		val = (t_valuemetadata *)l_val->content;
		i = 0;
		while (i < 8)
		{
			if (val->name[i] != 0)
				write(1, &val->name[i], 1);
			else
				write(1, &(".\0"), 1);
			i++;
		}
		i = 0;
		while (i < val->mem_offset)
		{
			printf("_");
			i++;
		}
		i = 0;
		while (i < val->size)
		{
			printf("#");
			i++;
		}
		printf("\n");
		l_val = l_val->next;
	}
}

void	print_analysis(t_structanalyzer *lyzer)
{
	t_structmetadata	*stru;
	t_valuemetadata		*val;
	t_list				*l;
	t_list				*l_val;

	l = lyzer->structs;
	while (l != NULL)
	{
		stru = (t_structmetadata *)l->content;
		printf("%s\n", stru->name);
		print_memorychunks(stru);
		/*l_val = stru->values;
		while (l_val != NULL) 
		{
			val = (t_valuemetadata *)l_val->content;
			printf("	%s	%s is %lu bytes\n", val->typename, val->name, val->size);
			l_val = l_val->next;
		}*/
		printf("\n");
		l = l->next;
	}
}

void	save_analysis(t_structanalyzer *saver)
{
	t_list				*l_sd;
	t_structmetadata		*sd;
	char				fancyname[32];

	l_sd = saver->structs;
	remove("test.structanalysis");
	while (l_sd != NULL)
	{
		sd = (t_structmetadata *)l_sd->content;
		ft_strcpy(fancyname, sd->name);
		ft_strcat(fancyname, "_fancy.txt");
		remove(sd->name);
		remove(fancyname);
		fancysave(fancyname, sd->values);
		force_save_chunk(sd->name, "VALS", sd->values);
		force_pack_file("test.structanalysis", sd->name);
		//printf("packing %s into main file\n", sd->name);
		l_sd = l_sd->next;
	}
}

void	applyoffsets(t_structanalyzer *lyzer)
{
	t_list				*l;
	t_list				*l_val;
	t_structmetadata	*stru;
	t_valuemetadata		*val;
	size_t				off;

	l = lyzer->structs;
	while (l != NULL)
	{
		stru = (t_structmetadata *)l->content;
		l_val = stru->values;
		off = 0;
		while (l_val != NULL)
		{
			val = (t_valuemetadata *)l_val->content;
			val->mem_offset = off;
			off += val->size;
			l_val = l_val->next;
		}
		l = l->next;
	}
}

void	read_header(char	*path, t_structanalyzer *lyzer)
{
	char			*line;
	bool			commented;
	int		fd;

	lyzer->reading = false;
	fd = open(path, O_RDONLY);
	if (fd != -1)
	{
		//printf("reading header: %s\n", path);
		commented = false;
		while (ft_get_next_line(fd, &line))
		{
			if (string_contains(line, "/*"))
				commented = true;
			if (string_contains(line, "*/"))
				commented = false;
			if (lyzer->reading == false)
			{
				char	*occ;
				occ = ft_strstr(line, "typedef struct ");
				if (occ != NULL)
				{
					ft_bzero(&lyzer->cur_struct, sizeof(lyzer->cur_struct));
					//ft_strcpy(saver.sdata.name, occ + sizeof("typedef struct"));
					//printf("parsing struct '%s'\n", saver.sdata.name);
					lyzer->reading = true;
				}
				continue ;
			}
			else
			{
				if (struct_ends(line))
				{
					set_structname(&lyzer->cur_struct, line);
					//printf("finished parsing struct %s with %i values\n", lyzer->cur_struct.name, ft_listlen(lyzer->cur_struct.values));
					list_push(&lyzer->structs, &lyzer->cur_struct, sizeof(lyzer->cur_struct));
					lyzer->reading = false;
					continue ;
				}
				if (commented || string_contains(line, "//"))
					continue ;
				try_parse_value(line, &lyzer->cur_struct);
			}
		}
	}
	close(fd);
	//printf("read %i struct datas \n", ft_listlen(lyzer->structs));
	//print_missing_types(lyzer->structs);
	t_structmetadata	*mis_struct = find_struct_match("t_container", lyzer);
	if (mis_struct != NULL)
	{
		//printf("trying to fix struct %s with missing values \n", mis_struct->name);
		fix_missing_types(lyzer, mis_struct);
		//print_missing_types(lyzer->structs);
	}
	applyoffsets(lyzer);

}

//void	print_structdata()

//void	load_struct

void	read_analysis_chunks(t_structanalyzer *lyzer, int fd)
{
	int				br;
	t_structmetadata	cur;
	char	buf[32 + 1];
	
	ft_bzero(buf, 4 + 1);
	br = read(fd, buf, 4);
	while (br > 0)
	{
		if (ft_strcmp("FCNK", buf) == 0)
		{
			read(fd, &cur, sizeof(cur));
			//printf("read %s analysis\n", cur.name);
			list_push(&lyzer->structs, &cur, sizeof(cur));
		}
		br = read(fd, buf, 4);
	}
}

void	read_analysis_variables(t_structanalyzer *lyzer)
{
	t_valuemetadata	*value;
	t_structmetadata	*s;
	t_list			*l;
	int				fd;

	l = lyzer->structs;
	while (l != NULL)
	{
		s = (t_structmetadata *)l->content;
		s->values = NULL;
		fd = load_filecontent_fd("test.structanalysis", s->name);
		close(fd);
		s->values = load_chunk(s->name, "VALS", sizeof(t_valuemetadata));
		//printf("loaded %i variables for struct %s\n", ft_listlen(s->values), s->name);
		//value = load_filecontent("test.structanalysis", s->name).content;
		l = l->next;
	}
	//valueload_filecontent()
}

t_structanalyzer	load_analysis()
{
	int					fd;
	t_structanalyzer	result;
	t_filecontent		fc;
	char				**chnk_names;
	int					i;

	ft_bzero(&result, sizeof(result));
	fd = open("test.structanalysis", O_RDONLY, 0666);
	if (fd == -1)
		return (result);
	read_analysis_chunks(&result, fd);
	close(fd);
	read_analysis_variables(&result);
	printf("OLD ANALYSIS:\n");
	//print_missing_types(result.structs);
	
	//load_fi
	return (result);
}

t_valuemetadata *find_value_match(t_structmetadata *stru, t_valuemetadata *match_val)
{
	t_list			*l_val;
	t_valuemetadata	*val;
	t_valuemetadata	*new_val;

	l_val = stru->values;
	while (l_val != NULL)
	{
		val = (t_valuemetadata *)l_val->content;
		if ((string_is_match(val->name, match_val->name)
			|| val->mem_offset == match_val->mem_offset) && val->size == match_val->size)
		{
			return (val);
		}
		l_val = l_val->next;
	}
	return (NULL);
}

t_list	*relocated_memory(t_structmetadata *old_md, t_structmetadata *new_md)
{
	t_list			*result;
	t_list			*l_val;
	t_valuemetadata	*val;
	t_valuemetadata	*new_val;

	l_val = old_md->values;
	while (l_val != NULL)
	{
		val = (t_valuemetadata *)l_val->content;
		//new_val = find_value_match(new_)
		//find_value_match(new_md)
		l_val = l_val->next;
	}
}

void	*structmetadata_allocate(void *oldmem, t_structmetadata	*stru_new, t_structmetadata	*stru_old)
{
	void	*mem;

	mem = ft_memalloc(sizeof_struct(stru_new));
	t_list			*l_val;
	t_valuemetadata	*val;
	t_valuemetadata	*new_val;

	l_val = stru_old->values;
	while (l_val != NULL)
	{
		val = (t_valuemetadata *)l_val->content;
		new_val = find_value_match(stru_new, val);
		if (new_val != NULL)
		{
			//mem
			ft_memcpy(mem + new_val->mem_offset, oldmem + val->mem_offset, val->size);
			//((char *)mem)[new_val->mem_offset] = ((char*)oldmem)[val->mem_offset];
			//mem[new_val->mem_offset] = 
			//sizeof(char *)
			printf("found match for %s \n", new_val->name);
		}
		//find_value_match(new_md)
		l_val = l_val->next;
	}
}

void	*load_singlechunk(char	*filename, char *chunkname, size_t size)
{
	t_list	*l;
	l = load_chunk(filename, chunkname, size);
	if (l != NULL)
		return (l->content);
	return (NULL);
}

void	load_struct(char	*filename, char *structname, t_structanalyzer *old, t_structanalyzer *new)
{
	void				*new_memory;
	void				*old_memory;
	t_structmetadata	*old_smd;
	t_structmetadata	*new_smd;

	if (old->structs == NULL || new->structs == NULL)
	{
		printf("cannot load struct %s because either metadata is totally missing\n", structname);
		return ;
	}
	old_smd = find_struct_match(structname, old);
	new_smd = find_struct_match(structname, new);
	if (structmetadata_has_missing_values(old_smd) || structmetadata_has_missing_values(new_smd))
	{
		printf("cannot load struct %s because it has missing values in metadata \n", structname);
		return ;
	}
	if (structmetadata_has_missing_values(old_smd) || structmetadata_has_missing_values(new_smd))
	{
		printf("cannot load struct %s because it has missing values in metadata \n", structname);
		return ;
	}
	old_memory = load_singlechunk("testfile", "TEST", sizeof_struct(old_smd));
	if (old_memory != NULL)
		structmetadata_allocate(old_memory, new_smd, old_smd);
	/*
	gather list of matching variables
	check for overlap?
	allocate memory from list of matching
	
	*/
	//new_memory = ft_memalloc(sizeof_struct(new_m));
	//printf("allocated %lu bytes for struct %s \n", sizeof_struct(new_m), structname);
	//old_m.
}

#include <test.h>

void	generate_struct_datas()
{
	return ;
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "include/";
	int				i;
	t_structanalyzer	old_analysis;
	t_structanalyzer	new_analysis;

	old_analysis = load_analysis(); //TODO: handle case with no old metadata
	/*d = opendir(path);
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".h") != NULL) //TODO: fix for windows compatability
			{
				read_header(ft_strcat(path, dfile->d_name));
				char *occ;
				occ = ft_strrchr(path, '/');
				if (occ != NULL)
					*(occ + 1) = 0;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}*/
	if (old_analysis.structs != NULL)
	{
		print_analysis(&old_analysis);
	}
	ft_bzero(&new_analysis, sizeof(new_analysis));
	//printf("NEW ANALYSIS:\n");
	read_header("include/test.h", &new_analysis);
	save_analysis(&new_analysis);
	printf("new analysis: \n");
	print_analysis(&new_analysis);
	//force_save_chunk("testfile", "TEST", )
	//load_struct("", "t_container", &old_analysis, &new_analysis);

	t_container cont = (t_container){.w = 13.2f, .z = -10.0f};
	exit(0);
}