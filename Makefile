# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/10/19 18:15:42 by okinnune         ###   ########.fr        #
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
		moveplayer.c physics.c errors.c game_3d.c fill_triangle.c \
		perfgraph.c png.c
VECTORSRCFILES= vector3_elementary.c vector3_shorthands.c \
		vector3_complex.c vector3_complex2.c \
		vector2_elementary.c vector2_shorthands.c \
		vector2_complex.c vector2_clamp.c \
		point_elementary.c point_shorthands.c \
		point_complex.c point_float_ops.c \
		conversions.c vector3_complex3.c \
		quaternion_shorthands.c quaternions.c \
		matrix_shorthands.c matrix_rotations.c \
		matrix_functions.c matrix_functions2.c \
		debug_vectors.c
VECTORSRC= $(addprefix src/vectors/,$(VECTORSRCFILES))
SRC= $(addprefix src/,$(SRCFILES))
SRC+= $(VECTORSRC)
OBJ= $(SRC:.c=.o)

#Compilation stuff:
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft #$(LIBFT)
CC= gcc
LIBS= $(LIBFT) -lm
CFLAGS= $(INCLUDE) -g -finline-functions -O1

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