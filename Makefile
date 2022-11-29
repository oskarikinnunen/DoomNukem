# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/11/23 15:12:56 by kfum             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME= DoomNukem
#Library dependencies:
SDLFOLDER= SDL-release-2.0.8
SDL2= SDL_built/lib/libSDL2.a
LIBFT= libft/libft.a

SRC_PATH = src/
OBJ_PATH = obj/
# VEC_PATH = vectors/
# EDITOR_PATH = editor/
# TOOL_PATH = tools/
# ENTITY_PATH = entity/
# OBJ_PAR_PATH = obj_parser/

LUAFOLDER= lua-5.3.6
LUA= $(LUAFOLDER)/install/lib/liblua.a #TODO: find out real name!

#Source files:
SRCFILES= main.c draw0.c draw1.c img.c deltatime.c anim.c \
		editor.c editor_mouse.c editor_events.c \
		editor_map_io.c	\
		editor/tools/entity_tool.c \
		editor/tools/wall_tool.c \
		editor/tools/button_tool.c \
		editor/imagedropdown.c \
		editor/editor_new_buttons.c \
		editor/editor_instant_button.c \
		editor/editor_movement.c \
		editor/editor_raycast.c \
		playmode.c inputhelper.c \
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
		
# VECTORSRC= $(addprefix src/vectors/,$(VECTORSRCFILES))
# SRC= $(addprefix src/,$(SRCFILES))
# SRC+= $(VECTORSRC)
# OBJ= $(SRC:.c=.o)

OBJ = $(patsubst %.c, $(OBJ_PATH)%.o, $(SRCFILES))
VECTORSRC= $(addprefix vectors/, $(VECTORSRCFILES))
VEC_OBJ = $(patsubst %.c, $(OBJ_PATH)%.o, $(VECTORSRC))

#Compilation stuff:
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include #$(LIBFT)
CC= gcc
LIBS= $(LIBFT) -lm -framework OpenGL
CFLAGS= $(INCLUDE) -g -finline-functions -O2#-march=native

all: $(SDL2) $(LUA) $(LIBFT) $(OBJ_PATH) $(OBJ) $(VEC_OBJ)
	$(CC) $(OBJ) $(VEC_OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBS) $(LUA)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)/editor/tools
	@mkdir -p $(OBJ_PATH)/entity
	@mkdir -p $(OBJ_PATH)/obj_parser
	@mkdir -p $(OBJ_PATH)/vectors

all: $(SDL2) $(LUA) $(LIBFT) $(OBJ_PATH) $(OBJ) $(VEC_OBJ)
	$(CC) $(OBJ) $(VEC_OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBS) $(LUA)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)/editor/tools
	@mkdir -p $(OBJ_PATH)/entity
	@mkdir -p $(OBJ_PATH)/obj_parser
	@mkdir -p $(OBJ_PATH)/vectors

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ): include/*.h Makefile

clean:
	rm -rf $(OBJ_PATH)

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
	cd $(SDLFOLDER)/build &&../configure --prefix=$(PWD)/SDL_built/ && make install && make install