# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2023/03/14 15:07:22 by vlaine           ###   ########.fr        #
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

#Source files:
SRCFILES= main.c img.c deltatime.c anim.c \
		file_ops/filechunks.c	\
		file_ops/world_save.c	\
		file_ops/world_load.c	\
		editor/editor.c \
		editor/editor_mouse.c \
		editor/editor_events.c \
		editor/editor_preferences.c \
		editor/editor_asset_load.c \
		editor/editor_load_anims.c \
		editor/editor_load_anim_legend.c \
		editor/editor_load_images.c \
		editor/editor_load_objects.c \
		editor/tools/entity_tool.c \
		editor/tools/entity_tool_gui_inits.c \
		editor/tools/wall_tool_rooms.c \
		editor/tools/npc_tool.c \
		editor/tools/load_tool.c \
		editor/tools/room_tool.c \
		editor/tools/room_tool_connect.c \
		editor/tools/room_tool_node.c \
		editor/tools/room_tool_addnode.c \
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
		debug/debug_rendering.c \
		inputhelper.c \
		walls.c file_open.c \
		moveplayer.c \
		collision.c \
		init_sdl.c \
		error_codes.c \
		error_messages.c \
		error_window.c \
		error_window_events.c \
		handle_exit.c \
		doomlog.c \
		doomlog_messages.c \
		physics.c \
		perfgraph.c \
		list_helper.c \
		spaceconversions.c \
		structsaver/structsaver.c \
		entity/entity_transform.c \
		entity/entity_animate.c \
		entity/component_init.c \
		entity/components/component_definitions.c \
		entity/components/comp_audiosource.c \
		entity/components/comp_audiosource_tools.c \
		entity/components/comp_interactable.c \
		physics/capsulephysics.c \
		player/playermovement_normal.c \
		player/playermovement_noclip.c \
		player/playmode.c \
		player/playmode_asset_load.c \
		player/playmode_death.c \
		player/playmode_events.c \
		player/hud.c \
		player/player.c \
		entity/components/comp_npc.c \
		guns/gun_presets.c \
		obj_parser/obj_parse.c \
		obj_parser/obj_parse_vertex.c \
		obj_parser/obj_parse_faces.c \
		obj_parser/obj_parse_uvs.c \
		tga_parser/tga.c \
		tga_parser/image_data.c \
		tga_parser/preliminary_data.c \
		game_random.c \
		font.c \
		object_init.c \
		object_primitives.c \
		world.c \
		resolution_scaling.c \
		controller.c \
		controller_init.c \
		controller_events.c \
		controller_events_buttons.c \
		audio_init.c \
		audio_tools.c \
		audio_sounds.c \
		audio_music.c \
		occlusion/occlusion.c \
		occlusion/frustrum_culling.c \
		occlusion/occlusion_culling.c \
		occlusion/culling_debug.c \
		occlusion/bitmask_culling.c \
		render/render_entity.c \
		render/render_clip.c \
		render/render_triangle.c \
		render/init_render.c \
		render/render_space.c \
		lighting/bake_lighting.c \
		lighting/point_light.c \
		lighting/fragment_light.c \
		npc/civilian/civilian_action.c \
		npc/civilian/civilian_anim.c \
		npc/civilian/civilian_movement.c \
		npc/civilian/civilian_update.c \
		npc/civilian/civilian_take_damage.c \
		npc/enemy/enemy_action.c \
		npc/enemy/enemy_anim.c \
		npc/enemy/enemy_movement.c \
		npc/enemy/enemy_update.c \
		npc/enemy/enemy_take_damage.c \
		npc/navigation/pathfind.c \
		npc/navigation/navmesh.c \
		render/rasterization/rasterize_zbuffer.c \
		render/rasterization/rasterize_triangle_wrap.c \
		render/rasterization/rasterize_triangle.c \
		render/draw.c \
		render/draw_rectangle.c \
		render/draw_shapes.c \
		render/render_helper.c \
		render/surface_tools.c \
		render/colors.c \
		render/text.c \
		render/rasterization/rasterize_triangle_shadow.c \
		render/render_zbuffer.c \
		render/pixel_utils.c \
		entity/components/comp_mechasuit.c \
		entity/components/comp_watercollider.c \
		entity/components/comp_npc_state.c \
		entity/components/comp_light.c \
		entity/components/comp_healthpack.c \
		entity/components/comp_npc_civilian.c \
		entity/components/comp_playerspawn.c \
		entity/components/comp_gun_pickup.c \
		entity/components/comp_auto_door.c #.ENDSRC. CREATECOMPONENT SCRIPT DEPENDS ON THIS SO DONT REMOVE
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
		matrix_transform.c \
		matrix_functions.c matrix_functions2.c \
		debug_vectors.c point_more.c \
		barycentric.c
VECTORSRC= $(addprefix src/vectors/,$(VECTORSRCFILES))
SRC= $(addprefix src/,$(SRCFILES))
SRC+= $(VECTORSRC)
OBJ= $(SRC:.c=.o)
DEPENDS= $(OBJ:.o=.d)

#Compilation stuff:
INCLUDE= -Isrc -Iinclude -Ilibft -I$(LUAFOLDER)/install/include \
			-I$(INSTALLED_LIBS_DIR)/include/SDL2/ \
			-I$(INSTALLED_LIBS_DIR)/include/FMOD/ #$(LIBFT)
CC= gcc
CFLAGS= $(INCLUDE) -g -finline-functions -O2 -MMD #-march=native
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

all: $(SDL2) $(FREETYPE) $(SDL2_TTF) $(FMOD) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(INCLUDE) $(LIBS) $(LDFLAGS)

#-include $(DEPENDS)

$(OBJ): Makefile include/*.h
#	//$()

clean:
	rm -f $(OBJ)
	rm -f $(DEPENDS)

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
