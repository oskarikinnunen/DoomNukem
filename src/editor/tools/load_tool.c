
#include "editor_tools.h"
#include "colliders.h"
#include "doomnukem.h"
#include <dirent.h>
#include "file_io.h" //For world load_args

typedef struct s_worldfile
{
	t_gamestring	name;
	bool			has_amap;
	bool			has_basic_ent;
	bool			has_full_ent;
}	t_worldfile;

typedef struct s_loadtooldata
{
	t_autogui	gui;
	char		**world_strings;
	int			world_count;
	t_worldfile	files[128];
	uint32_t	file_count;
}	t_loadtooldata;

char	*_fname_strip_prefix(char *fname, char	*prefix)
{
	static t_gamestring	gs;
	char			*occ;

	
	ft_strcpy(gs.str, fname);
	occ = ft_strstr(gs.str, prefix);
	if (occ != NULL)
		*occ = '\0';
	return (gs.str);
}

static void	_lt_worldfile_add_amap(t_loadtooldata *dat, char	*fname)
{
	int	i;
	char	*wf_name;

	i = 0;
	wf_name = _fname_strip_prefix(fname, ".amap");
	while (i < dat->file_count) //TODO: Protect for stack size
	{
		if (ft_strequ(wf_name, dat->files[i].name.str))
		{
			dat->files[i].has_amap = true;
			return ;
		}
		i++;
	}
	ft_strcpy(dat->files[i].name.str, wf_name);
	dat->files[i].has_amap = true;
	printf("found new file %s with amap \n", dat->files[i].name.str);
	dat->file_count++;
}

static void	_lt_find_worldfiles(t_loadtooldata	*dat)
{
	DIR				*d;
	int				i;
	struct dirent	*dfile;

	d = opendir("worlds");
	i = 0;
	if (d)
	{
		dfile = readdir(d);
		while (dfile != NULL)
		{
			if (dfile->d_type == DT_REG && ft_strstr(dfile->d_name, ".amap") != NULL)
				_lt_worldfile_add_amap(dat, dfile->d_name);
			i++;
			dfile = readdir(d);
		}
		closedir(d);
	}
	//dat->world_count = i;
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

/*
Editor mode can load
	.amap
	.basic_ent
	.full_ent (with sanitized room entities)

Playmode loads
	world->roomlist isn't available to playmode
	.level (which contains:)
		.full_ent (room entities are baked into this)
		.lmaps
		.colliders
*/

void	load_tool_init(t_editor *ed, t_sdlcontext *sdl)
{
	t_loadtooldata	*dat;
	DIR				*d;
	struct dirent	*dfile;
	char path		[256] = "worlds";
	char fullpath	[512];
	int				i;

	dat = ed->tool->tooldata;
	_lt_find_worldfiles(dat);
	if (dat->gui.sdl == NULL)
	{
		dat->gui = init_gui(ed->world.sdl, &ed->hid, &ed->player, (t_point){200, 200}, "Load/Save levels");
		dat->gui.rect.size = dat->gui.minimum_size;
		dat->gui.rect.size.y = 400;
	}
	return ;
}

static char	*file_status_str(t_worldfile	file)
{
	static char	status[8];

	ft_strcpy(status, "[");
	if (file.has_amap)
		ft_strcat(status, "A");
	if (file.has_basic_ent)
		ft_strcat(status, ",B");
	if (file.has_full_ent)
		ft_strcat(status, ",F");
	ft_strcat(status, "]");
	return (status);
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
	gui_label("World name:", gui);
	gui->offset.x = 20;
	gui_string_edit(ed->world.name, gui);
	gui->offset.x = 0;
	if (gui_button("Save current world", gui))
	{
		world_save_to_file(ed->world);
		ed->world.lastsavetime = ed->world.clock.prev_time;
		return ;
		load_tool_init(ed, sdl);
		debugconsole_addmessage(&ed->world.debugconsole, "Saved world!");
	}
	if (gui_button("Save backup", gui))
	{
		char tempname[32];
		ft_bzero(tempname, sizeof(tempname));
		ft_strcpy(tempname, ed->world.name);
		char *ext = ft_strstr(tempname, ".world");
		*ext = '\0';
		ft_strcat(tempname, "_bu.world");
		//save_world(tempname, ed->world);
		world_save_to_file(ed->world);
		load_tool_init(ed, sdl);
		return ;
		debugconsole_addmessage(&ed->world.debugconsole, "Saved backup world!");
	}
	gui_emptyvertical(10, gui);
	gui_label("Load:", gui);
	
	while (i < dat->file_count)
	{
		gui_starthorizontal(gui);
		if (gui_button(dat->files[i].name.str, gui))
		{
			editor_load_world_args(ed, dat->files[i].name.str, ed->world.sdl, LOAD_ARG_FULL);
			return ;
		}
		gui_label(file_status_str(dat->files[i]), gui);
		gui_endhorizontal(gui);
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
