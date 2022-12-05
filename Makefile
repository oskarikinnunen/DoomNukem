# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/12/05 14:32:57 by okinnune         ###   ########.fr        #
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
SRCFILES= main.c draw0.c draw1.c img.c deltatime.c anim.c \
		editor.c editor_mouse.c editor_events.c \
		editor_map_io.c	\
		editor/tools/entity_tool.c \
		editor/tools/wall_tool_rooms.c \
		editor/tools/npc_tool.c \
		editor/tools/room_tool.c \
		editor/tools/gun_tool.c \
		editor/tools/tool_common_functions.c \
		editor/tools/autogui.c \
		editor/editor_new_buttons.c \
		editor/editor_instant_button.c \
		editor/editor_raycast.c \
		debug/debugconsole.c \
		playmode.c inputhelper.c \
		walls.c file_open.c \
		moveplayer.c errors.c \
		physics.c \
		game_3d.c fill_triangle.c perfgraph.c \
		png.c lua_conf.c list_helper.c \
		spaceconversions.c \
		entity/entity_animate.c \
		obj_parser/obj_parse.c \
		obj_parser/obj_parse_vertex.c \
		obj_parser/obj_parse_faces.c \
		obj_parser/obj_parse_uvs.c \
		obj_parser/obj_parse_anim.c\
		font.c text.c \
		object_init.c \
		object_primitives.c \
		world.c player.c \
		init_render.c \
		controller.c \
		occlusion/occlusion.c occlusion/frustrum_culling.c occlusion/peripheral_culling.c occlusion/occlusion_culling.c
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
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include #$(LIBFT)
CC= gcc

CFLAGS= $(INCLUDE) -g -Ofast#-march=native
UNAME= $(shell uname)
ifeq ($(UNAME), Darwin)
override CFLAGS += '-D GL_SILENCE_DEPRECATION'
LIBS= $(LIBFT) -lm -framework OpenGL
else ifeq ($(UNAME), Linux)
LIBS =  $(LIBFT) -lm -lGL
else
warning:
	@echo "Compilation for platform $(UNAME) not supported."
	exit 1
endif


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