
#include "editor_tools.h"
#include "file_io.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"


// TODO: Protect?
void	editor_load_prefs(t_editor *ed, t_sdlcontext *sdl)
{
	char			*prefpath;
	char			pref_filename[256];
	t_editorprefs	prefs;
	int				fd;

	ft_bzero(pref_filename, sizeof(pref_filename));
	ft_bzero(&prefs, sizeof(t_editorprefs));
	prefpath = SDL_GetPrefPath("temp", "stark");
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDONLY);
	if (fd != -1)
	{
		read(fd, &prefs, sizeof(t_editorprefs));
		close(fd);
		ed->player.transform.position = prefs.playerpos;
		doomlog_mul(LOG_NORMAL, (char *[5]){"loaded prefs from", prefpath, \
			"- worldname:", prefs.worldname, NULL});
		editor_load_world_args(ed, prefs.worldname, sdl, LOAD_ARG_FULL);
	}
	else
		editor_load_world_args(ed, "default", sdl, LOAD_ARG_FULL);
	SDL_free(prefpath);
}

void	editor_save_prefs(t_editor *ed)
{
	char			*prefpath;
	char			pref_filename[256];
	t_editorprefs	prefs;
	int				fd;

	ft_bzero(pref_filename, sizeof(pref_filename));
	ft_bzero(&prefs, sizeof(t_editorprefs));
	prefpath = SDL_GetPrefPath("temp", "stark");
	ft_strcpy(pref_filename, prefpath);
	ft_strcat(pref_filename, "editor.pref");
	fd = open(pref_filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd != -1)
	{
		ft_strcpy(prefs.worldname, ed->world.name);
		prefs.playerpos = ed->player.transform.position;
		write(fd, &prefs, sizeof(prefs));
		close(fd);
		doomlog_mul(LOG_NORMAL, (char *[5]){\
			"saved prefs to", pref_filename, \
			"worldname:", prefs.worldname, NULL});
	}
	SDL_free(prefpath);
}

#pragma GCC diagnostic pop