# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raho <raho@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/11/21 20:33:23 by raho             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME= DoomNukem
#Library dependencies:
SDLFOLDER= SDL-release-2.0.8
SDL2= SDL_built/lib/libSDL2.a
LIBFT= libft/libft.a

FREETYPE_DIR = freetype-2.9
FREETYPE = freetype_built/lib/libfreetype.a
SDL2_TTF_DIR = SDL2_ttf-2.0.15
SDL2_TTF = SDL2_ttf_built/lib/libSDL2_ttf.a

LUAFOLDER= lua-5.3.6
LUA= $(LUAFOLDER)/install/lib/liblua.a #TODO: find out real name!

#Source files:
SRCFILES= main.c draw0.c draw1.c img.c deltatime.c anim.c \
		editor.c editor_mouse.c editor_events.c \
		editor_map_io.c	\
		editor/tools/entity_tool.c \
		editor/tools/wall_tool.c \
		editor/tools/wall_tool_rooms.c \
		editor/tools/button_tool.c \
		editor/imagedropdown.c \
		editor/editor_new_buttons.c \
		editor/editor_instant_button.c \
		editor/editor_movement.c \
		editor/editor_raycast.c \
		playmode.c inputhelper.c \
		walls.c \
		moveplayer.c physics.c errors.c \
		game_3d.c fill_triangle.c perfgraph.c \
		png.c lua_conf.c list_helper.c \
		spaceconversions.c \
		obj_parser/obj_parse.c \
		obj_parser/obj_parse_vertex.c \
		obj_parser/obj_parse_faces.c \
		obj_parser/obj_parse_uvs.c \
		font.c text.c \
		object_init.c \
		object_primitives.c \
		world.c \
		init_render.c \
		controller.c
VECTORSRCFILES= vector3_elementary.c vector3_shorthands.c \
		vector3_complex.c vector3_complex2.c \
		vector2_elementary.c vector2_shorthands.c \
		vector2_complex.c vector2_clamp.c \
		vector2_more.c \
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
INCLUDE= -ISDL_built/include/SDL2/ -ISDL2_ttf_built/include/SDL2/ -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include #$(LIBFT)
CC= gcc
LIBS= $(LIBFT) -lm
CFLAGS= $(INCLUDE) -g -finline-functions -O2

all: $(SDL2) $(FREETYPE) $(SDL2_TTF) $(LUA) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` -LSDL2_ttf_built/lib -lSDL2_ttf $(INCLUDE) $(LIBS) $(LUA)

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

$(FREETYPE):
	cd $(FREETYPE_DIR) && ./configure --prefix=$(PWD)/freetype_built/ && make install

$(SDL2_TTF):
	cd $(SDL2_TTF_DIR) && ./configure --prefix=$(PWD)/SDL2_ttf_built/ --with-ft-prefix=$(PWD)/freetype_built --with-sdl-prefix=$(PWD)/SDL_built/ && make install
