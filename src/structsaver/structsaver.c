/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structsaver.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 22:11:46 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/16 22:26:38 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "libft.h"
#include <dirent.h>

typedef enum e_valuetype
{
	value_float,
	value_int
}	t_valuetype;

typedef struct s_savedvalue
{
	char		name[32];
	t_valuetype	type;
	size_t		size;
}	t_savedvalue;

typedef struct s_savedstruct
{
	char		structname[32];
	uint64_t	id;
}	t_savedstruct;

typedef struct s_struct_savedata
{
	char			name[32];
	t_list			*values;
}	t_struct_savedata;



void	generate_struct_datas()
{
	/*DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "include/objects";
	int				i;

	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".obj") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	sdl->objectcount = i;
	printf("Found %i .obj files \n", sdl->objectcount);
	sdl->objects = ft_memalloc(sizeof(t_object) * sdl->objectcount);*/
}