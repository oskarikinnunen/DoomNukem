/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structsaver.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 22:11:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/03/01 18:58:10 by okinnune         ###   ########.fr       */
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
#include "structsaver.h"

//TODO:
/*
	enums,
	different int sizes
*/

bool	string_contains(char *haystack, char *needle)
{
	return (ft_strstr(haystack, needle) != NULL);
}

bool	string_starts_with(char *string1, char *startswith)
{
	return (ft_strncmp(string1, startswith, ft_strlen(startswith)) == 0);
}

bool	string_is_match(char *string1, char *string2)
{
	return (ft_strcmp(string1, string2) == 0);
}

typedef struct s_structanalyzer
{
	bool				reading;
	t_structmetadata	cur_struct;
	t_list				*structs;
	t_list				*enums;
}	t_structanalyzer;

bool	struct_ends(char *line)
{
	return (string_contains(line, "}") && string_contains(line, ";"));
}

t_structmetadata	*find_struct_match(char *structname, t_structanalyzer *saver);

bool	structmetadata_has_missing_values(t_structanalyzer *analyzer, t_structmetadata *smd)
{
	t_list				*l_val;
	t_valuemetadata		*val;
	t_structmetadata	*child;

	l_val = smd->values;
	while (l_val != NULL)
	{
		val = (t_valuemetadata *)l_val->content;
		if (val->type == value_missing)
			return (true);
		if (val->type == value_struct)
		{
			/*child = find_struct_match(val->typename, analyzer);
			if (child != NULL && structmetadata_has_missing_values(analyzer, child))
				return (true);*/
		}
		l_val = l_val->next;
	}
	return (false);
}

void	set_structname(t_structmetadata *saved, char *line)
{
	char	*a;
	int		i;

	i = 0;
	char	*occ;
	occ = ft_strstr(line, "__attribute__((packed, aligned(1))");
	if (occ != NULL)
	{
		line += sizeof("__attribute__((packed, aligned(1)))");
	}
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
		if (string_is_match(split[0], "char") || string_is_match(split[0], "uint8_t") || string_is_match(split[0], "int8_t") || string_contains(split[0], "bool"))
		{
			value.size = 1UL;
			value.type = value_char;
		}
		else if (string_is_match(split[0], "float"))
		{
			value.size = 4UL;
			value.type = value_float;
		}
		else if (string_contains(split[0], "int"))
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
		if (value.type != value_missing && string_contains(split[1], "[") && string_contains(split[1], "]"))
		{
			char arrsize_str[32];
			char *arrsize_start = ft_strstr(split[1], "[");
			ft_strcpy(arrsize_str, arrsize_start + 1);
			size_t arrsize = ft_atoi(arrsize_str);
			printf("found array of size %i \n", arrsize);
			if (arrsize > 0)
				value.size = value.size * arrsize;
		}
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
		doomlog(LOG_NORMAL, sd->name);
		l_val = sd->values;
		i = 0;
		while (l_val != NULL)
		{
			val = (t_valuemetadata *)l_val->content;
			if (val->type == value_missing)
			{
				doomlog_mul(LOG_WARNING, (char *[32]){"missing value:", val->typename, NULL});
				i++;
			}
				
			l_val = l_val->next;
		}
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
	//if (size % 8 != 0)
	//t_gun gun;
	//offsetof(gun.active, gun);
	//	size += size % 8;
	return (size);
}

size_t	sizeof_internal_struct(t_structmetadata *saved)
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
	//sizeof(t_anim);
	return (size);
}

bool	is_enum(t_structanalyzer *analyzer, char *val_typename)
{
	t_list	*l;
	char	*enumname;

	l = analyzer->enums;
	while (l != NULL)
	{
		enumname = (char *)l->content;
		if (string_is_match(val_typename + 2, enumname)) //TODO: protect short enumname
		{
			return (true);
		}
		l = l->next;
	}
	return (false);
}

void	fix_missing_types(t_structanalyzer	*saver, t_structmetadata	*saved)
{
	t_list				*l_sd;
	t_list				*l_val;
	t_valuemetadata		*val;
	t_structmetadata		*sd;

	l_sd = saver->structs;
	doomlog_mul(LOG_WARNING, (char *[32]){"Fixing struct ", saved->name, NULL});
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
				if (val->type != value_ptr && string_is_match(val->typename, sd->name))
				{
					val->size = sizeof_internal_struct(sd);
					val->type = value_struct;
					//printf("found missing value type %s as struct of size %lu \n", val->name, val->size);
				}
				if (is_enum(saver, val->typename))
				{
					doomlog_mul(LOG_WARNING, (char *[32]){"Found enum ", val->typename, NULL});
					val->size = sizeof(int32_t);
					val->type = value_int;
				}
				l_val = l_val->next;
			}
		}
		/*if(string_is_match(sd->name, saved))
			return (sd);*/
		l_sd = l_sd->next;
	}
}

void	fix_all_missing_types(t_structanalyzer	*saver)
{
	t_list				*l_sd;
	t_list				*l_val;
	t_valuemetadata		*val;
	t_structmetadata		*sd;

	l_sd = saver->structs;
	while (l_sd != NULL) 
	{
		sd = (t_structmetadata *)l_sd->content;
		if (structmetadata_has_missing_values(saver, sd))
			fix_missing_types(saver, sd);
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
	write(1, stru->name, 32);
	write(1, &"\n", 1);
	printf("total size: %i \n", sizeof_struct(stru));
	while (l_val != NULL) 
	{
		val = (t_valuemetadata *)l_val->content;
		i = 0;
		while (i < 16)
		{
			if (val->name[i] != 0)
				write(1, &val->name[i], 1);
			else
				write(1, &(".\0"), 1);
			i++;
		}
		i = 0;
		printf("OFFSET %i, SIZE %i \n", val->mem_offset, val->size);
		/*while (i < val->mem_offset)
		{
			printf("_");
			i++;
		}
		i = 0;
		while (i < val->size)
		{
			printf("#");
			i++;
		}*/
		//printf("\n");
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
	print_missing_types(l);
	while (l != NULL)
	{
		stru = (t_structmetadata *)l->content;
		if (!structmetadata_has_missing_values(lyzer, stru))
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

void	analyzer_check_enum(t_structanalyzer *analyzer, char *line)
{
	char	*occ;
	char	*enumname;

	occ = ft_strstr(line, "typedef enum");
	if (occ != NULL)
	{
		enumname = line + sizeof("typedef enum e");
		list_push(&analyzer->enums, enumname, ft_strlen(enumname) + 1);
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
					lyzer->reading = true;
				}
				analyzer_check_enum(lyzer, line);
				continue ;
			}
			else
			{
				if (struct_ends(line))
				{
					set_structname(&lyzer->cur_struct, line);
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
	fix_all_missing_types(lyzer);
	applyoffsets(lyzer);
}

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

t_structanalyzer *old_struct_analysis() //TODO: handle case where there is no old analysis
{
	static t_structanalyzer	old_analysis;
	if (old_analysis.structs == NULL)
	{
		old_analysis = load_analysis();
	}
	return (&old_analysis);
}

t_structanalyzer *new_struct_analysis()
{
	static t_structanalyzer	new_analysis;
	if (new_analysis.structs == NULL)
	{
		read_header("include/vectors.h", &new_analysis);
		read_header("include/animation.h", &new_analysis);
		read_header("include/components.h", &new_analysis);
		read_header("include/entity.h", &new_analysis);
		t_list *enumlist = new_analysis.enums;
		while (enumlist != NULL)
		{
			char *str = enumlist->content;
			doomlog_mul(LOG_WARNING, (char *[32]){"Enum def ", str, NULL});
			enumlist = enumlist->next;
		}
	}
	return (&new_analysis);
}
//load_struct(entity_0001_component, componentname)
void	load_struct(char	*filename, char *structname)
{
	void				*new_memory;
	void				*old_memory;
	t_structmetadata	*old_smd;
	t_structmetadata	*new_smd;
	t_structanalyzer	*old;
	t_structanalyzer	*new;

	old = old_struct_analysis();
	new = new_struct_analysis();
	if (old->structs == NULL || new->structs == NULL)
	{
		printf("cannot load struct %s because either metadata is totally missing\n", structname);
		return ;
	}
	old_smd = find_struct_match(structname, old);
	new_smd = find_struct_match(structname, new);
	/*if (structmetadata_has_missing_values(old_smd) || structmetadata_has_missing_values(new_smd))
	{
		printf("cannot load struct %s because it has missing values in metadata \n", structname);
		return ;
	}*/
	//old_memory = load_singlechunk("testfile", "TEST", sizeof_struct(old_smd));
	//load_filecontent_fd()
	if (old_memory != NULL)
		structmetadata_allocate(old_memory, new_smd, old_smd);
}

//.world
//	.amap
//	.fent
//	.global

//filename, packname, structname, structdata
//save_struct("gun_1", game_global, "t_gun", (void *)gun)

//save_struct("entity_1_component", world, component.name, (void *)component.data)

void	save_struct(char	filename[32], char *packname, char *structname, void *structdata)
{
	int					fd;
	t_structanalyzer	*new;
	t_structmetadata	*s_meta;

	new = new_struct_analysis();
	fd = fileopen(filename, O_CREAT | O_RDWR | O_APPEND);
	write(fd, structname, ft_strlen(structname));
	s_meta = find_struct_match(structname, new);
	if (s_meta != NULL)
	{
		if (structmetadata_has_missing_values(new_struct_analysis, s_meta))
		{
			doomlog(LOG_WARNING, "Can't save struct since it has missing values.");
		}
		if (sizeof(t_gun) != sizeof_struct(find_struct_match("t_gun", new)))
		{
			doomlog(LOG_WARNING, "Can't save struct since metadata doesn't match the real size.");
		}
		//sizeof(t_entity **);
		//sizeof(t_gun);
		sizeof(char[128]);
		print_analysis(new);
	}
	sizeof(t_gun);
	/*
	Decide if this always packs this somewhere
	
	*/
	close(fd);
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
	read_header("include/test.h", &new_analysis);
	save_analysis(&new_analysis);
	printf("new analysis: \n");
	print_analysis(&new_analysis);
	//sizeof(t_gun);
	sizeof(t_anim);
	//force_save_chunk("testfile", "TEST", )
	//load_struct("", "t_container", &old_analysis, &new_analysis);

	t_container cont = (t_container){.w = 13.2f, .z = -10.0f};
	exit(0);
}