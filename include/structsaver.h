#ifndef STRUCTSAVER_H
# define STRUCTSAVER_H

#include <inttypes.h>
#include "libft.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <doomnukem.h>
#include <file_io.h>

typedef enum e_valuetype
{
	value_float,
	value_int,
	value_char,
	value_struct,
	value_ptr,
	value_missing
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

#endif