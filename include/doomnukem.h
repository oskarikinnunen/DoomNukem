/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomnukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:39:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/03 14:11:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../SDL_built/include/SDL2/SDL.h" //TODO: make this work without relative path?
#include "limits.h" //TODO: remove probably, used for INT_MAX


#define WINDOW_W 800
#define WINDOW_H 600
#define TILESIZE 32 //EDITOR tilesize
#define GRIDSIZE 16 //EDITOR tilesize
#define X 0
#define Y 1



typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	//t_simpleimg				*images;
}	t_sdlcontext;

/* EDITOR.C */
void	editorloop(t_sdlcontext sdl);