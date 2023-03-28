# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kfum <kfum@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2023/03/28 11:52:43 by kfum             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = doom-nukem
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
SRC_PATH = src/
OBJ_PATH = obj/

#Source files:
SRCFILES= main.c img.c deltatime.c \
		animations/anim.c \
		animations/anim_parse.c \
		load_assets/asset_tools.c \
		load_assets/editor_load_assets.c \
		load_assets/editor_load_anims.c \
		load_assets/editor_load_anim_legend.c \
		load_assets/editor_load_images.c \
		load_assets/editor_load_objects.c \
		load_assets/editor_load_fonts.c \
		load_assets/editor_load_sounds.c \
		load_assets/editor_load_music.c \
		load_assets/playmode_load_assets.c \
		load_assets/playmode_load_images.c \
		load_assets/playmode_load_env_textures.c \
		load_assets/playmode_load_objects.c \
		load_assets/playmode_load_fonts.c \
		load_assets/playmode_load_sounds.c \
		load_assets/playmode_load_music.c \
		load_assets/playmode_load_anims.c \
		load_assets/playmode_load_anim_legend.c \
		loading_screens/loading_screen.c \
		file_ops/file_chunks.c \
		file_ops/file_tools.c \
		file_ops/file_content.c \
		file_ops/world_save.c \
		file_ops/world_save_basic_ent.c \
		file_ops/world_save_amap.c \
		file_ops/world_load.c \
		file_ops/world_init.c \
		gui/gui_buttons1.c \
		gui/gui_buttons2.c \
		gui/gui_end.c \
		gui/gui_internal.c \
		gui/gui_labeled_elements.c \
		gui/gui_labels.c \
		gui/gui_presets.c \
		gui/gui_set_up.c \
		gui/gui_sliders1.c \
		gui/gui_sliders2.c \
		gui/gui_tools1.c \
		gui/gui_tools2.c \
		gui/gui_tools3.c \
		editor/editor.c \
		editor/editor_mouse.c \
		editor/editor_events.c \
		editor/editor_set_up.c \
		editor/key_events.c \
		editor/tools/entity_tool.c \
		editor/tools/entity_tool_base.c \
		editor/tools/entity_tool_modify.c \
		editor/tools/entity_tool_modify_2.c \
		editor/tools/entity_tool_gui_inits.c \
		editor/tools/wall_tool_rooms.c \
		editor/tools/floor_triangulate.c \
		editor/tools/floor_triangulate_2.c \
		editor/tools/floor_triangulate_3.c \
		editor/tools/floor_triangulate_4.c \
		editor/tools/room_ops.c \
		editor/tools/room_ops_2.c \
		editor/tools/room_tool.c \
		editor/tools/room_tool_modify.c \
		editor/tools/room_tool_modify_2.c \
		editor/tools/room_tool_create.c \
		editor/tools/room_tool_raycast_room.c \
		editor/tools/room_tool_init_and_get.c \
		editor/tools/room_tool_connect.c \
		editor/tools/room_tool_node.c \
		editor/tools/room_tool_node_2.c \
		editor/tools/room_tool_node_3.c \
		editor/tools/room_tool_addnode.c \
		editor/tools/room_tool_common.c \
		editor/tools/room_tool_common_2.c \
		editor/tools/room_tool_paint.c \
		editor/tools/room_tool_paint_2.c \
		editor/tools/gun_tool.c \
		editor/tools/gun_tool_init.c \
		editor/tools/gun_tool_update.c \
		editor/editor_new_buttons.c \
		editor/editor_raycast.c \
		editor/entity_raycast.c \
		debug/debug_rendering.c \
		inputhelper.c \
		walls.c walls_2.c walls_3.c file_open.c \
		moveplayer.c \
		init_sdl.c \
		logging/log_tools.c \
		logging/error_codes.c \
		logging/error_messages.c \
		logging/doomlog.c \
		logging/doomlog_messages.c \
		linked_list/list_tools1.c \
		linked_list/list_tools2.c \
		spaceconversions.c \
		entity/component_init.c \
		entity/components/component_definitions.c \
		entity/components/comp_audiosource.c \
		entity/components/comp_audiosource_setup.c \
		entity/components/comp_audiosource_tools.c \
		entity/components/comp_interactable.c \
		entity/components/comp_interactable_2.c \
		physics/capsule_physics.c \
		physics/capsule_physics_2.c \
		physics/capsule_physics_3.c \
		physics/capsule_physics_4.c \
		physics/capsule_physics_step.c \
		physics/collision_character.c \
		physics/collision_line_circle.c \
		physics/collision_simple.c \
		physics/collision_point_triangle.c \
		physics/collision_line_line_intersect.c \
		player/playermovement_normal.c \
		player/playermovement_normal1.c \
		player/playermovement_normal2.c \
		player/playermovement_noclip.c \
		player/playmode_set_up.c \
		player/playmode_protect.c \
		player/playmode.c \
		player/playmode_death.c \
		player/playmode_events.c \
		player/hud.c \
		player/player.c \
		player/player_2.c \
		entity/components/comp_npc.c \
		entity/components/comp_npc_get_hit.c \
		entity/components/comp_npc_state.c \
		entity/components/comp_npc_states_1.c \
		entity/components/comp_npc_raycast.c \
		entity/components/comp_npc_funcs.c \
		entity/components/comp_npc_helpers.c \
		entity/components/comp_npc_anims.c \
		guns/gun_presets.c \
		guns/gun_funcs.c \
		guns/gun_transform.c \
		objects/obj_parse.c \
		objects/obj_parse_vertex.c \
		objects/obj_parse_faces.c \
		objects/obj_parse_uvs.c \
		objects/obj_tools.c \
		objects/mat_tools.c \
		objects/mat_parse.c \
		tga_parser/tga.c \
		tga_parser/image_data.c \
		tga_parser/preliminary_data.c \
		game_random.c \
		logging/prot_memalloc.c \
		objects/object_init.c \
		objects/object_primitives.c \
		resolution_scaling.c \
		controller/controller.c \
		controller/controller_init.c \
		controller/controller_events.c \
		controller/controller_events_buttons.c \
		audio/audio_init.c \
		audio/audio_tools.c \
		audio/audio_sounds.c \
		audio/audio_update.c \
		occlusion/occlusion.c \
		occlusion/frustrum_culling.c \
		occlusion/occlusion_culling.c \
		occlusion/culling_debug.c \
		occlusion/bitmask_culling.c \
		decal/decal.c \
		lighting/allocate_map.c \
		lighting/bake_lighting.c \
		lighting/calculate_lightmap.c \
		lighting/calculate_texture.c \
		lighting/point_light.c \
		lighting/fragment_light.c \
		lighting/rasterize_lighting/render_zbuffer.c \
		lighting/rasterize_lighting/rasterize_zbuffer.c \
		lighting/rasterize_lighting/rasterize_triangle_shadow.c \
		lighting/rasterize_lighting/rasterize_texture.c \
		lighting/rasterize_lighting/rasterize_light_utils.c \
		thread/thread.c \
		thread/thread_lighting.c \
		navigation/pathfind.c \
		navigation/navmesh.c \
		navigation/create_navmesh.c \
		navigation/nav_utils.c \
		navigation/connect_navmesh.c \
		render/rasterization/rasterize_triangle_wrap.c \
		render/rasterization/rasterize_triangle.c \
		render/rasterization/rasterize_triangle_transparent.c \
		render/draw.c \
		render/draw2.c \
		render/draw_rectangle.c \
		render/draw_shapes.c \
		render/draw_skybox.c \
		render/render_helper.c \
		render/surface_tools.c \
		render/colors.c \
		render/text.c \
		render/pixel_utils.c \
		render/render_dynamic_light.c \
		render/render_entity.c \
		render/renders_world_triangles_to_screen_triangles.c \
		render/render_buffer.c \
		render/init_render.c \
		render/gizmos/render_gizmo.c \
		render/gizmos/render_ball.c \
		render/gizmos/clip_quatline.c \
		render/transform/transform_triangle.c \
		render/transform/transform_quaternion.c \
		render/transform/clip_world_triangle_to_plane.c \
		render/transform/clip_screen_triangle_to_plane.c \
		render/transform/clip_screen_triangle_to_screen.c \
		render/transform/transform_bounds.c \
		world/entitycache.c \
		world/entitycache_2.c \
		world/entitycache_transparent.c \
		world/world.c \
		entity/components/comp_light.c \
		entity/components/comp_healthpack.c \
		entity/components/comp_player_spawn1.c \
		entity/components/comp_player_spawn2.c \
		entity/components/comp_gun_pickup1.c \
		entity/components/comp_gun_pickup2.c \
		entity/components/comp_auto_door.c \
		entity/components/comp_story_events1.c \
		entity/components/comp_story_events2.c \
		entity/components/comp_boutton1.c \
		entity/components/comp_boutton2.c #.ENDSRC. CREATECOMPONENT SCRIPT DEPENDS ON THIS SO DONT REMOVE
VECTORSRCFILES= conversions.c \
		conversions2.c \
		matrix_functions3.c \
		matrix_functions2.c \
		matrix_functions.c \
		matrix_rotations.c \
		matrix_shorthands.c \
		matrix_transform.c \
		point_complex2.c \
		point_complex.c \
		point_elementary.c \
		point_float_ops.c \
		point_more.c \
		point_shorthands.c \
		quaternions.c \
		quaternion_shorthands.c \
		vector2_clamp.c \
		vector2_complex2.c \
		vector2_complex3.c \
		vector2_complex.c \
		vector2_elementary.c \
		vector2_more.c \
		vector2_shorthands.c \
		vector2_round.c \
		vector3_cmp.c \
		vector3_complex2.c \
		vector3_complex3.c \
		vector3_complex.c \
		vector3_elementary.c \
		vector3_more.c \
		vector3_shorthands.c \
		vector3_shorthands2.c \
		barycentric.c \
		triangle_functions.c \
		triangle_functions2.c

OBJ = $(patsubst %.c, $(OBJ_PATH)%.o, $(SRCFILES))
VECTORSRC= $(addprefix vectors/, $(VECTORSRCFILES))
VEC_OBJ = $(patsubst %.c, $(OBJ_PATH)%.o, $(VECTORSRC))

#Compilation stuff:
INCLUDE= -Isrc -Iinclude -Ilibft -I$(INSTALLED_LIBS_DIR)/include/SDL2/ \
			-I$(INSTALLED_LIBS_DIR)/include/FMOD/
CC= gcc
CFLAGS= $(INCLUDE) -g -finline-functions -O3
LDFLAGS = -Wl,-rpath $(INSTALLED_LIBS_DIR)/lib

UNAME= $(shell uname)
ifeq ($(UNAME), Darwin)
LIBS= $(LIBFT) -lm -L$(INSTALLED_LIBS_DIR)/lib -lSDL2 -lSDL2_ttf -L$(INSTALLED_LIBS_DIR)/lib -lfmod -lfmodL
AUTOGEN =
else ifeq ($(UNAME), Linux)
LIBS =  $(LIBFT) -lm -L$(INSTALLED_LIBS_DIR)/lib -lSDL2 -lSDL2_ttf -L$(INSTALLED_LIBS_DIR)/lib -lfmod -lfmodL -lpthread
AUTOGEN = ./autogen.sh &&
else
warning:
	@echo "Compilation for platform $(UNAME) not supported."
	exit 1
endif

all: $(NAME)

$(NAME): $(SDL2) $(FREETYPE) $(SDL2_TTF) $(FMOD) $(LIBFT) $(OBJ_PATH) $(OBJ) $(VEC_OBJ)
	$(CC) $(OBJ) $(VEC_OBJ) -o $(NAME) $(INCLUDE) $(LIBS) $(LDFLAGS)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)/animations
	@mkdir -p $(OBJ_PATH)/audio
	@mkdir -p $(OBJ_PATH)/controller
	@mkdir -p $(OBJ_PATH)/debug
	@mkdir -p $(OBJ_PATH)/decal
	@mkdir -p $(OBJ_PATH)/editor
	@mkdir -p $(OBJ_PATH)/editor/tools
	@mkdir -p $(OBJ_PATH)/entity
	@mkdir -p $(OBJ_PATH)/entity/components
	@mkdir -p $(OBJ_PATH)/file_ops
	@mkdir -p $(OBJ_PATH)/gui
	@mkdir -p $(OBJ_PATH)/guns
	@mkdir -p $(OBJ_PATH)/lighting
	@mkdir -p $(OBJ_PATH)/lighting/rasterize_lighting
	@mkdir -p $(OBJ_PATH)/linked_list
	@mkdir -p $(OBJ_PATH)/load_assets
	@mkdir -p $(OBJ_PATH)/loading_screens
	@mkdir -p $(OBJ_PATH)/logging
	@mkdir -p $(OBJ_PATH)/navigation
	@mkdir -p $(OBJ_PATH)/objects
	@mkdir -p $(OBJ_PATH)/obj_parser
	@mkdir -p $(OBJ_PATH)/occlusion
	@mkdir -p $(OBJ_PATH)/physics
	@mkdir -p $(OBJ_PATH)/player
	@mkdir -p $(OBJ_PATH)/render
	@mkdir -p $(OBJ_PATH)/render/gizmos
	@mkdir -p $(OBJ_PATH)/render/rasterization
	@mkdir -p $(OBJ_PATH)/render/transform
	@mkdir -p $(OBJ_PATH)/tga_parser
	@mkdir -p $(OBJ_PATH)/thread
	@mkdir -p $(OBJ_PATH)/vectors
	@mkdir -p $(OBJ_PATH)/world

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ): Makefile include/*.h

clean:
	@echo "$(RED)\nCleaning obj folder...$(DEFAULT)"
	@rm -rf $(OBJ_PATH)
	@echo "$(GREEN)DONE.\n$(DEFAULT)"

fclean: clean
	@echo "$(RED)Deleting .$(NAME)...$(DEFAULT)" 
	@rm -f $(NAME)
	@echo "$(GREEN)DONE.\n$(DEFAULT)"

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

# Output colors
DEFAULT	:=\033[0m
GREEN	:=\033[0;32m
RED		:=\033[0;31m