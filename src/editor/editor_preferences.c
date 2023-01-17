
#include "editor_tools.h"
#include "file_io.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

void	editor_load_prefs(t_editor *ed, t_sdlcontext *sdl)
{
	char			*prefpath;
	char			*pref_filename;
	t_editorprefs	prefs;
	int				fd;
	prefpath = SDL_GetPrefPath("temp", "stark");
	pref_filename = ft_strnew(256);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDONLY);
	if (fd != -1)
	{
		read(fd, &prefs, sizeof(t_editorprefs));
		ed->player.transform.position = prefs.playerpos;
		editor_load_and_init_world(ed, prefs.worldname, sdl);
		printf("loading prefs from '%s' , worldname %s \n", prefpath, prefs.worldname);
	}
	else
		editor_load_and_init_world(ed, "worlds/test123.world", sdl);
	SDL_free(prefpath);
	free(pref_filename);
}

void	editor_save_prefs(t_editor *ed)
{
	char			*prefpath;
	char			*pref_filename;
	t_editorprefs	prefs;
	int				fd;
	prefpath = SDL_GetPrefPath("temp", "stark");
	ft_bzero(&prefs, sizeof(prefs));
	
	pref_filename = ft_strnew(256);
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd != -1)
	{
		ft_strcpy(prefs.worldname, ed->world.name);
		prefs.playerpos = ed->player.transform.position;
		write(fd, &prefs, sizeof(prefs));
		//editor_load_and_init_world(ed, prefs.worldname, sdl);
		close(fd);
		printf("saved prefs to '%s', worldname was %s \n", pref_filename, prefs.worldname);
	}
	SDL_free(prefpath);
	free(pref_filename);
}

#pragma GCC diagnostic pop