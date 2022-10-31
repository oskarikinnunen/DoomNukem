# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/10/31 19:55:26 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME= DoomNukem
#Library dependencies:
SDLFOLDER= SDL-release-2.0.8
SDL2= SDL_built/lib/libSDL2.a
LIBFT= libft/libft.a

LUAFOLDER= lua-5.3.6
LUA= $(LUAFOLDER)/install/lib/liblua.a #TODO: find out real name!

#Source files:
SRCFILES= main.c draw.c img.c deltatime.c anim.c\
		editor.c editor_mouse.c editor_events.c editor_render.c \
		editor_map_io.c	\
		editor/tools/point_tool.c \
		editor/tools/point_tool_delete.c \
		editor/tools/entity_tool.c \
		editor/tools/button_tool.c \
		editor/imagedropdown.c \
		editor/editor_new_buttons.c \
		playmode.c inputhelper.c playmode_overhead.c \
		moveplayer.c physics.c errors.c \
		game_3d.c fill_triangle.c perfgraph.c \
		png.c lua_conf.c list_helper.c \
		spaceconversions.c \
		obj_parser/obj_parse.c \
		obj_parser/obj_parse_vertex.c \
		obj_parser/obj_parse_faces.c \
		obj_parser/obj_parse_uvs.c \
		object_init.c
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
		debug_vectors.c point_more.c
VECTORSRC= $(addprefix src/vectors/,$(VECTORSRCFILES))
SRC= $(addprefix src/,$(SRCFILES))
SRC+= $(VECTORSRC)
OBJ= $(SRC:.c=.o)

#Compilation stuff:
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include #$(LIBFT)
CC= gcc
LIBS= $(LIBFT) -lm
CFLAGS= $(INCLUDE) -g -finline-functions -O1

all: $(SDL2) $(LUA) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBS) $(LUA)

$(OBJ): include/*.h Makefile

clean:
	rm -f $(OBJ)

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

clean-lua:
	rm -rf $(LUAFOLDER)/install

re-lua: clean-lua $(LUA)

$(LUA):
	cd $(LUAFOLDER) && make generic && make local

$(SDL2):
	cd $(SDLFOLDER)/build &&../configure --prefix=$(PWD)/SDL_built/ && make install