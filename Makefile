# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2023/01/11 14:54:50 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = DoomNukem
#Library dependencies:

LIBS_DIR = libs
INSTALLED_LIBS_DIR = $(LIBS_DIR)/installed_libs

SDL2_DIR = $(LIBS_DIR)/SDL2-2.0.8
SDL2_TTF_DIR = $(LIBS_DIR)/SDL2_ttf-2.0.15
FREETYPE_DIR = $(LIBS_DIR)/freetype-2.9
FMOD_DIR = $(INSTALLED_LIBS_DIR)/lib/FMOD

SDL2 = $(INSTALLED_LIBS_DIR)/lib/libSDL2.a
SDL2_TTF = $(INSTALLED_LIBS_DIR)/lib/libSDL2_ttf.a
FREETYPE = $(INSTALLED_LIBS_DIR)/lib/libfreetype.a
FMOD = $(FMOD_DIR)/copied

LIBFT = libft/libft.a

LUAFOLDER= lua-5.3.6
LUA= $(LUAFOLDER)/install/lib/liblua.a

#Source files:
SRCFILES= main.c draw0.c img.c deltatime.c anim.c \
		editor.c editor_mouse.c editor_events.c \
		editor_map_io.c	\
		editor/tools/entity_tool.c \
		editor/tools/wall_tool_rooms.c \
		editor/tools/npc_tool.c \
		editor/tools/load_tool.c \
		editor/tools/room_tool.c \
		editor/tools/room_tool_connect.c \
		editor/tools/room_tool_common.c \
		editor/tools/room_tool_paint.c \
		editor/tools/gun_tool.c \
		editor/tools/tool_common_functions.c \
		editor/tools/autogui.c \
		editor/editor_new_buttons.c \
		editor/editor_instant_button.c \
		editor/editor_raycast.c \
		preferences/graphicsprefs.c \
		debug/debugconsole.c \
		playmode.c inputhelper.c \
		walls.c file_open.c \
		moveplayer.c errors.c \
		physics.c \
		perfgraph.c \
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
		resolution_scaling.c \
		controller.c \
		audio_init.c \
		audio_tools.c \
		audio_sounds.c \
		audio_music.c \
		occlusion/occlusion.c \
		occlusion/frustrum_culling.c \
		occlusion/occlusion_culling.c \
		occlusion/culling_debug.c \
		occlusion/bitmask_culling.c \
		surface_tools.c \
		colors.c \
		render/render_entity.c \
		render/render_clip.c \
		render/render_triangle.c \
		render/init_render.c \
		render/render_space.c \
		lighting/bake_lighting.c \
		lighting/point_light.c \
		render/rasterization/rasterize_triangle_wrap.c \
		render/rasterization/rasterize_triangle.c \
		render/rasterization/rasterize_triangle_uv.c \
		render/rasterization/rasterize_triangle_dynamic.c \
		render/render_helper.c \
		render/flip_channel.c
VECTORSRCFILES= vector3_elementary.c vector3_shorthands.c \
		vector3_complex.c vector3_complex2.c vector3_more.c \
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
INCLUDE= -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include \
			-I$(INSTALLED_LIBS_DIR)/include/SDL2/ \
			-I$(INSTALLED_LIBS_DIR)/include/FMOD/ #$(LIBFT)
CC= gcc
CFLAGS= $(INCLUDE) -g -finline-functions -O2 #-march=native
LDFLAGS = -Wl,-rpath $(INSTALLED_LIBS_DIR)/lib

UNAME= $(shell uname)
ifeq ($(UNAME), Darwin)
override CFLAGS += '-D GL_SILENCE_DEPRECATION'
LIBS= $(LIBFT) -lm -framework OpenGL -L$(INSTALLED_LIBS_DIR)/lib -lSDL2 -lSDL2_ttf -L$(INSTALLED_LIBS_DIR)/lib -lfmod -lfmodL
AUTOGEN =
else ifeq ($(UNAME), Linux)
LIBS =  $(LIBFT) -lm -lGL -L$(INSTALLED_LIBS_DIR)/lib -lSDL2 -lSDL2_ttf -L$(INSTALLED_LIBS_DIR)/lib -lfmod -lfmodL
AUTOGEN = ./autogen.sh &&
else
warning:
	@echo "Compilation for platform $(UNAME) not supported."
	exit 1
endif

#multi:
#	$(MAKE) -j6 all

all: $(SDL2) $(FREETYPE) $(SDL2_TTF) $(FMOD) $(LUA) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(INCLUDE) $(LIBS) $(LUA) $(LDFLAGS)

$(OBJ): include/*.h Makefile

clean:
	rm -f $(OBJ)

re: clean all

$(LIBFT):
	make -C libft

clean-libs:
	rm -rf $(SDL2_DIR) $(FREETYPE_DIR) $(SDL2_TTF_DIR) \
		$(INSTALLED_LIBS_DIR)/bin $(INSTALLED_LIBS_DIR)/include/freetype2 \
		$(INSTALLED_LIBS_DIR)/include/SDL2 $(INSTALLED_LIBS_DIR)/lib/cmake \
		$(INSTALLED_LIBS_DIR)/lib/pkgconfig $(INSTALLED_LIBS_DIR)/lib/lib* \
		$(INSTALLED_LIBS_DIR)/share
	rm $(FMOD)

re-libs: clean-libs all

clean-lua:
	rm -rf $(LUAFOLDER)/install

re-lua: clean-lua $(LUA)

$(LUA):
	cd $(LUAFOLDER) && make generic && make local

$(FMOD):
	cp $(FMOD_DIR)/libfmod.dylib $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmodL.dylib $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmod.so $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmod.so.13 $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmod.so.13.11 $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmodL.so $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmodL.so.13 $(INSTALLED_LIBS_DIR)/lib/
	cp $(FMOD_DIR)/libfmodL.so.13.11 $(INSTALLED_LIBS_DIR)/lib/
	touch $(FMOD)

$(SDL2_DIR)/unpacked:
	cd $(LIBS_DIR) && tar -xf SDL2-2.0.8.tar.gz
	cd $(SDL2_DIR) && touch unpacked

$(FREETYPE_DIR)/unpacked:
	cd $(LIBS_DIR) && tar -xf freetype-2.9.tar.gz
	cd $(FREETYPE_DIR) && touch unpacked

$(SDL2_TTF_DIR)/unpacked:
	cd $(LIBS_DIR) && tar -xf SDL2_ttf-2.0.15.tar.gz
	cd $(SDL2_TTF_DIR) && touch unpacked


$(SDL2_DIR)/ready_to_build: $(SDL2_DIR)/unpacked
	cd $(SDL2_DIR) && ./configure --prefix=$(PWD)/$(INSTALLED_LIBS_DIR) && touch ready_to_build

$(FREETYPE_DIR)/ready_to_build: $(FREETYPE_DIR)/unpacked
	cd $(FREETYPE_DIR) && ./configure --prefix=$(PWD)/$(INSTALLED_LIBS_DIR) && touch ready_to_build

# On Linux autogen.sh will be executed in SDL2_TTF_DIR before running configure and make install
# On Linux pkg-config overrides prefixes with default path so we change the PKG_CONFIG_PATH
$(SDL2_TTF_DIR)/ready_to_build: $(SDL2_TTF_DIR)/unpacked
	cd $(SDL2_TTF_DIR) && $(AUTOGEN) ./configure	\
	--prefix=$(PWD)/$(INSTALLED_LIBS_DIR)	\
	--with-ft-prefix=$(PWD)/$(INSTALLED_LIBS_DIR)	\
	--with-sdl-prefix=$(PWD)/$(INSTALLED_LIBS_DIR)	\
	PKG_CONFIG_PATH=$(PWD)/$(INSTALLED_LIBS_DIR)/lib/pkgconfig	\
	&& touch ready_to_build

$(SDL2): $(SDL2_DIR)/ready_to_build
	cd $(SDL2_DIR) && make && make install

$(FREETYPE): $(FREETYPE_DIR)/ready_to_build
	cd $(FREETYPE_DIR) && make && make install

$(SDL2_TTF): $(SDL2_TTF_DIR)/ready_to_build
	cd $(SDL2_TTF_DIR) && make && make install
