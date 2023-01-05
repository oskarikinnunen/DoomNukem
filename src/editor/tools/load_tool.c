
#include "editor_tools.h"
#include "colliders.h"
#include "doomnukem.h"
#include <dirent.h>


typedef struct s_loadtooldata
{
	char	**world_strings;
	int		world_count;
}	t_loadtooldata;

void	allocate_worldstrings(t_loadtooldata	*dat, char	*path)
{
	DIR				*d;
	int				i;
	struct dirent	*dfile;

	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".world") != NULL)
				i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	dat->world_count = i;
	dat->world_strings = ft_memalloc(sizeof(char *) * dat->world_count);
	printf("allocced space for %i worlds \n", i);
}

void	load_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_loadtooldata	*dat;
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "worlds";
	char fullpath	[512];
	int				i;

	dat = ed->tool->tooldata;
	allocate_worldstrings(dat, path);
	d = opendir(path);
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".world") != NULL)
			{
				dat->world_strings[i] = ft_strnew(32);
				ft_strcpy(dat->world_strings[i], dfile->d_name);
				printf("world %i string '%s' loaded \n", i, dat->world_strings[i]);
				//sdl->objects[i] = objparse(fullpath);
				//ft_strcpy(sdl->objects[i].name, dfile->d_name);
				//printf("	parsed objectfile: %s \n", fullpath);
				i++;
			}
			dfile = readdir(d);
		}
		closedir(d);
	}
}

void	load_tool_update(t_editor *ed, t_sdlcontext *sdl)
{

}

t_tool	*get_load_tool()
{
	static t_tool	tool
	= {
		.update = load_tool_update, .init = load_tool_init//.cleanup = npc_tool_cleanup
	};
	//t_npctooldata	*dat;
	t_entity		*ent;

	if (tool.tooldata == NULL)
	{
		tool.tooldata = ft_memalloc(sizeof(t_loadtooldata));
		/*dat->ent->transform.position = vector3_zero();
		dat->ent->transform.scale = vector3_one();*/
	}
	return (&tool);
}
