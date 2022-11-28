# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raho <raho@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/11/28 19:19:49 by raho             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = DoomNukem
#Library dependencies:

LIBS_DIR = libs
SDL2_DIR = $(LIBS_DIR)/SDL2-2.0.8
SDL2_TTF_DIR = $(LIBS_DIR)/SDL2_ttf-2.0.15
FREETYPE_DIR = $(LIBS_DIR)/freetype-2.9
INSTALLED_LIBS_DIR = $(LIBS_DIR)/installed_libs
SDL2 = $(INSTALLED_LIBS_DIR)/lib/libSDL2.a
SDL2_TTF = $(INSTALLED_LIBS_DIR)/lib/libSDL2_ttf.a
FREETYPE = $(INSTALLED_LIBS_DIR)/lib/libfreetype.a
LIBFT = libft/libft.a



LUAFOLDER= lua-5.3.6
LUA= $(LUAFOLDER)/install/lib/liblua.a #TODO: find out real name!

#Source files:
SRCFILES= main.c draw0.c draw1.c img.c deltatime.c anim.c \
		editor.c editor_mouse.c editor_events.c \
		editor_map_io.c	\
		editor/tools/entity_tool.c \
		editor/tools/wall_tool.c \
		editor/tools/wall_tool_rooms.c \
		editor/tools/npc_tool.c \
		editor/tools/room_tool.c \
		editor/tools/button_tool.c \
		editor/imagedropdown.c \
		editor/editor_new_buttons.c \
		editor/editor_instant_button.c \
		editor/editor_movement.c \
		editor/editor_raycast.c \
		playmode.c inputhelper.c \
		walls.c file_open.c \
		moveplayer.c physics.c errors.c \
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
		world.c \
		init_render.c \
		controller.c \
		audio.c \
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
INCLUDE= -I$(INSTALLED_LIBS_DIR)/include/SDL2/ -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include #$(LIBFT)
CC= gcc
CFLAGS= $(INCLUDE) -g -finline-functions -O2#-march=native

UNAME= $(shell uname)
ifeq ($(UNAME), Darwin)
override CFLAGS += '-D GL_SILENCE_DEPRECATION'
LIBS= $(LIBFT) -lm -framework OpenGL `$(INSTALLED_LIBS_DIR)/bin/sdl2-config --cflags --libs` -L$(INSTALLED_LIBS_DIR)/lib -lSDL2_ttf
AUTOGEN =
else ifeq ($(UNAME), Linux)
LIBS =  $(LIBFT) -lm -lGL `$(INSTALLED_LIBS_DIR)/bin/sdl2-config --cflags --libs` -L$(INSTALLED_LIBS_DIR)/lib -lSDL2_ttf
AUTOGEN = ./autogen.sh &&
else
warning:
	@echo "Compilation for platform $(UNAME) not supported."
	exit 1
endif


all: $(SDL2) $(FREETYPE) $(SDL2_TTF) $(LUA) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(INCLUDE) $(LIBS) $(LUA)

$(OBJ): include/*.h Makefile

clean:
	rm -f $(OBJ)

re: clean all

$(LIBFT):
	make -C libft

clean-libs:
	rm -rf $(INSTALLED_LIBS_DIR)

re-libs: clean-libs $(SDL2) $(FREETYPE) $(SDL2_TTF)

clean-lua:
	rm -rf $(LUAFOLDER)/install

re-lua: clean-lua $(LUA)

$(LUA):
	cd $(LUAFOLDER) && make generic && make local

$(SDL2_DIR)/unpacked:
	cd $(LIBS_DIR) && tar -xf SDL2-2.0.8.tar.gz
	cd $(SDL2_DIR) && touch unpacked

$(FREETYPE_DIR)/unpacked:
	cd $(LIBS_DIR) && tar -xf freetype-2.9.tar.gz
	cd $(FREETYPE_DIR) && touch unpacked

$(SDL2_TTF_DIR)/unpacked:
	cd $(LIBS_DIR) && tar -xf SDL2_ttf-2.0.15.tar.gz
	cd $(SDL2_TTF_DIR) && touch unpacked


$(SDL2_DIR)/configured: $(SDL2_DIR)/unpacked
	cd $(SDL2_DIR) && ./configure --prefix=$(PWD)/$(INSTALLED_LIBS_DIR) SDL_AUDIODRIVER=pulseaudio && touch configured

$(FREETYPE_DIR)/configured: $(FREETYPE_DIR)/unpacked
	cd $(FREETYPE_DIR) && ./configure --prefix=$(PWD)/$(INSTALLED_LIBS_DIR) && touch configured

# On Linux autogen.sh will be executed in SDL2_TTF_DIR before running configure and make install
# On Linux pkg-config overrides prefixes with default path so we change the PKG_CONFIG_PATH
$(SDL2_TTF_DIR)/configured: $(SDL2_TTF_DIR)/unpacked
	cd $(SDL2_TTF_DIR) && $(AUTOGEN) ./configure	\
	--prefix=$(PWD)/$(INSTALLED_LIBS_DIR)	\
	--with-ft-prefix=$(PWD)/$(INSTALLED_LIBS_DIR)	\
	--with-sdl-prefix=$(PWD)/$(INSTALLED_LIBS_DIR)	\
	PKG_CONFIG_PATH=$(PWD)/$(INSTALLED_LIBS_DIR)/lib/pkgconfig	\
	&& touch configured

$(SDL2): $(SDL2_DIR)/configured
	cd $(SDL2_DIR) && make && make install

$(FREETYPE): $(FREETYPE_DIR)/configured
	cd $(FREETYPE_DIR) && make && make install

$(SDL2_TTF): $(SDL2_TTF_DIR)/configured
	cd $(SDL2_TTF_DIR) && make && make install
