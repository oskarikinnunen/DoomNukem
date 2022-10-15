# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raho <raho@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
 #    Updated: 2022/10/12 16:05:30 by raho             ###   ########.fr        #
=======
#    Updated: 2022/10/15 12:33:33 by okinnune         ###   ########.fr        #
>>>>>>> vector2-rewrite
#                                                                              #
# **************************************************************************** #


NAME= DoomNukem
#Library dependencies:
SDLFOLDER= SDL-release-2.0.8
SDL2= SDL_built/lib/libSDL2.a
LIBFT= libft/libft.a

#Source files:
SRCFILES= main.c draw.c img.c deltatime.c anim.c\
		editor.c editor_mouse.c editor_events.c editor_render.c \
		editor_saveline.c editor_map_io.c\
		playmode.c inputhelper.c playmode_overhead.c \
		moveplayer.c physics.c
VECTORSRCFILES= vector3_elementary.c vector3_shorthands.c \
		vector3_complex.c \
		vector2_elementary.c vector2_shorthands.c \
		vector2_complex.c vector2_clamp.c \
		point_elementary.c point_shorthands.c \
		point_complex.c point_float_ops.c \
		conversions.c \
		quaternion_shorthands.c 
VECTORSRC= $(addprefix src/vectors/,$(VECTORSRCFILES))
SRC= $(addprefix src/,$(SRCFILES))
SRC+= $(VECTORSRC)
OBJ= $(SRC:.c=.o)

#Compilation stuff:
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft #$(LIBFT)
CC= gcc
LIBS= $(LIBFT) -lm
CFLAGS= $(INCLUDE) -g

all: $(SDL2) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBS)

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