
#include "editor_tools.h"
#include "colliders.h"
#include "doomnukem.h"
#include <dirent.h>


typedef struct s_loadtooldata
{
	t_autogui	gui;
	char		**world_strings;
	int			world_count;
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

void	load_tool_free(t_loadtooldata *dat)
{
	int	i;

	i = 0;
	if (dat->world_count != 0)
	{
		while (i < dat->world_count)
		{
			free(dat->world_strings[i]);
			i++;
		}
		free(dat->world_strings);
	}
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
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".world\0") != NULL)
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
	if (dat->gui.sdl == NULL)
	{
		dat->gui = init_gui(ed->world.sdl, &ed->hid, &ed->player, (t_point){200, 200}, "Load/Save levels");
		dat->gui.rect.size = dat->gui.minimum_size;
		dat->gui.rect.size.y = 400;
	}
}

void	load_tool_update(t_editor *ed, t_sdlcontext *sdl)
{
	t_loadtooldata	*dat;
	t_autogui		*gui;
	static char		name[32];
	int				i;

	dat = ed->tool->tooldata;
	gui = &dat->gui;
	i = 0;
	gui_start(gui);
	gui_string_edit(ed->world.name, gui);
	if (gui_button("Save current world", gui))
	{
		save_world(ed->world.name, ed->world);
		load_tool_init(ed, sdl);
	}
	if (gui_button("Save backup", gui))
	{
		char tempname[32];
		ft_bzero(tempname, sizeof(tempname));
		ft_strcpy(tempname, ed->world.name);
		char *ext = ft_strstr(tempname, ".world");
		*ext = '\0';
		ft_strcat(tempname, "_bu.world");
		save_world(tempname, ed->world);
		load_tool_init(ed, sdl);
	}
	gui_emptyvertical(10, gui);
	gui_label("Load:", gui);
	while (i < dat->world_count)
	{
		if (ft_strcmp(dat->world_strings[i], ed->world.name) != 0 &&
			gui_button(dat->world_strings[i], gui))
			editor_load_and_init_world(ed, dat->world_strings[i], ed->world.sdl);
		i++;
	}
	gui_end(gui);

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
