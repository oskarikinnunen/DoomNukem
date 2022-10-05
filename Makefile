# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/10/05 14:25:53 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME= DoomNukem
#Library dependencies:
SDLFOLDER= SDL-release-2.0.22
SDL2= SDL_built/lib/libSDL2.a
LIBFT= libft/libft.a

#Source files:
SRCFILES= main.c draw.c v2.c img.c fdf.c deltatime.c anim.c\
		editor.c editor_mouse.c editor_events.c editor_render.c \
		editor_saveline.c editor_map_io.c editor_3d_initialize.c \
		editor_3d.c
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)

#Compilation stuff:
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft $(LIBFT)
CC= gcc
CFLAGS= $(INCLUDE) -lm -g

all: $(SDL2) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE)

$(OBJ): include/doomnukem.h

clean:
	rm -f src/*.o

re: clean all

$(LIBFT):
	make -C libft

clean-sdl:
	rm -rf $(SDLFOLDER)/build/*
	touch $(SDLFOLDER)/build/DontRemoveMe
	rm -rf SDL_built/*
	touch SDL_built/DontRemoveMe
	rm -f $(SDL2)

re-sdl: clean-sdl $(SDL2)

$(SDL2):
	cd $(SDLFOLDER)/build &&../configure --prefix=$(PWD)/SDL_built/ && make install