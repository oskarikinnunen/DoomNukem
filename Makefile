# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raho <raho@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2023/03/27 17:20:38 by raho             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = doom-nukem
#Library dependencies:

LIBS_DIR = libs
INSTALLED_LIBS_DIR = $(LIBS_DIR)/installed_libs

SDL2_DIR = $(LIBS_DIR)/SDL-release-2.30.0#/SDL2-2.0.8
SDL2_TTF_DIR = $(LIBS_DIR)/SDL2_ttf-2.0.15
FREETYPE_DIR = $(LIBS_DIR)/freetype-2.9
FMOD_DIR = $(INSTALLED_LIBS_DIR)/lib/FMOD

SDL2 = $(INSTALLED_LIBS_DIR)/lib/libSDL2.a
SDL2_TTF = $(INSTALLED_LIBS_DIR)/lib/libSDL2_ttf.a
FREETYPE = $(INSTALLED_LIBS_DIR)/lib/libfreetype.a
FMOD = $(FMOD_DIR)/copied

LIBFT = libft/libft.a

#Source files:
SRCFILES= main.cpp img.cpp deltatime.cpp \
		animations/anim.cpp \
		animations/anim_parse.cpp \
		load_assets/asset_tools.cpp \
		load_assets/editor_load_assets.cpp \
		load_assets/editor_load_anims.cpp \
		load_assets/editor_load_anim_legend.cpp \
		load_assets/editor_load_images.cpp \
		load_assets/editor_load_objects.cpp \
		load_assets/editor_load_fonts.cpp \
		load_assets/editor_load_sounds.cpp \
		load_assets/editor_load_music.cpp \
		load_assets/playmode_load_assets.cpp \
		load_assets/playmode_load_images.cpp \
		load_assets/playmode_load_env_textures.cpp \
		load_assets/playmode_load_objects.cpp \
		load_assets/playmode_load_fonts.cpp \
		load_assets/playmode_load_sounds.cpp \
		load_assets/playmode_load_music.cpp \
		load_assets/playmode_load_anims.cpp \
		load_assets/playmode_load_anim_legend.cpp \
		loading_screens/loading_screen.cpp \
		file_ops/file_chunks.cpp \
		file_ops/file_tools.cpp \
		file_ops/file_content.cpp \
		file_ops/world_save.cpp \
		file_ops/world_save_basic_ent.cpp \
		file_ops/world_save_amap.cpp \
		file_ops/world_load.cpp \
		file_ops/world_init.cpp \
		gui/gui_buttons1.cpp \
		gui/gui_buttons2.cpp \
		gui/gui_end.cpp \
		gui/gui_internal.cpp \
		gui/gui_labeled_elements.cpp \
		gui/gui_labels.cpp \
		gui/gui_presets.cpp \
		gui/gui_set_up.cpp \
		gui/gui_sliders1.cpp \
		gui/gui_sliders2.cpp \
		gui/gui_tools1.cpp \
		gui/gui_tools2.cpp \
		gui/gui_tools3.cpp \
		editor/editor.cpp \
		editor/editor_mouse.cpp \
		editor/editor_events.cpp \
		editor/editor_set_up.cpp \
		editor/key_events.cpp \
		editor/tools/entity_tool.cpp \
		editor/tools/entity_tool_base.cpp \
		editor/tools/entity_tool_modify.cpp \
		editor/tools/entity_tool_modify_2.cpp \
		editor/tools/entity_tool_gui_inits.cpp \
		editor/tools/wall_tool_rooms.cpp \
		editor/tools/floor_triangulate.cpp \
		editor/tools/floor_triangulate_2.cpp \
		editor/tools/floor_triangulate_3.cpp \
		editor/tools/floor_triangulate_4.cpp \
		editor/tools/room_ops.cpp \
		editor/tools/room_ops_2.cpp \
		editor/tools/room_tool.cpp \
		editor/tools/room_tool_modify.cpp \
		editor/tools/room_tool_modify_2.cpp \
		editor/tools/room_tool_create.cpp \
		editor/tools/room_tool_raycast_room.cpp \
		editor/tools/room_tool_init_and_get.cpp \
		editor/tools/room_tool_connect.cpp \
		editor/tools/room_tool_node.cpp \
		editor/tools/room_tool_node_2.cpp \
		editor/tools/room_tool_node_3.cpp \
		editor/tools/room_tool_addnode.cpp \
		editor/tools/room_tool_common.cpp \
		editor/tools/room_tool_common_2.cpp \
		editor/tools/room_tool_paint.cpp \
		editor/tools/room_tool_paint_2.cpp \
		editor/tools/gun_tool.cpp \
		editor/tools/gun_tool_init.cpp \
		editor/tools/gun_tool_update.cpp \
		editor/editor_new_buttons.cpp \
		editor/editor_raycast.cpp \
		editor/entity_raycast.cpp \
		debug/debug_rendering.cpp \
		inputhelper.cpp \
		walls.cpp walls_2.cpp walls_3.cpp file_open.cpp \
		moveplayer.cpp \
		init_sdl.cpp \
		logging/log_tools.cpp \
		logging/error_codes.cpp \
		logging/error_messages.cpp \
		logging/doomlog.cpp \
		logging/doomlog_messages.cpp \
		linked_list/list_tools1.cpp \
		linked_list/list_tools2.cpp \
		spaceconversions.cpp \
		entity/component_init.cpp \
		entity/components/component_definitions.cpp \
		entity/components/comp_audiosource.cpp \
		entity/components/comp_audiosource_setup.cpp \
		entity/components/comp_audiosource_tools.cpp \
		entity/components/comp_interactable.cpp \
		entity/components/comp_interactable_2.cpp \
		physics/capsule_physics.cpp \
		physics/capsule_physics_2.cpp \
		physics/capsule_physics_3.cpp \
		physics/capsule_physics_4.cpp \
		physics/capsule_physics_step.cpp \
		physics/collision_character.cpp \
		physics/collision_line_circle.cpp \
		physics/collision_simple.cpp \
		physics/collision_point_triangle.cpp \
		physics/collision_line_line_intersect.cpp \
		player/playermovement_normal.cpp \
		player/playermovement_normal1.cpp \
		player/playermovement_normal2.cpp \
		player/playermovement_noclip.cpp \
		player/playmode_set_up.cpp \
		player/playmode.cpp \
		player/playmode_death.cpp \
		player/playmode_events.cpp \
		player/hud.cpp \
		player/player.cpp \
		player/player_2.cpp \
		entity/components/comp_npc.cpp \
		entity/components/comp_npc_get_hit.cpp \
		entity/components/comp_npc_state.cpp \
		entity/components/comp_npc_states_1.cpp \
		entity/components/comp_npc_raycast.cpp \
		entity/components/comp_npc_funcs.cpp \
		entity/components/comp_npc_helpers.cpp \
		entity/components/comp_npc_anims.cpp \
		guns/gun_presets.cpp \
		guns/gun_funcs.cpp \
		guns/gun_transform.cpp \
		objects/obj_parse.cpp \
		objects/obj_parse_vertex.cpp \
		objects/obj_parse_faces.cpp \
		objects/obj_parse_uvs.cpp \
		objects/obj_tools.cpp \
		objects/mat_tools.cpp \
		objects/mat_parse.cpp \
		tga_parser/tga.cpp \
		tga_parser/image_data.cpp \
		tga_parser/preliminary_data.cpp \
		game_random.cpp \
		logging/prot_memalloc.cpp \
		objects/object_init.cpp \
		objects/object_primitives.cpp \
		resolution_scaling.cpp \
		controller/controller.cpp \
		controller/controller_init.cpp \
		controller/controller_events.cpp \
		controller/controller_events_buttons.cpp \
		audio/audio_init.cpp \
		audio/audio_tools.cpp \
		audio/audio_sounds.cpp \
		audio/audio_update.cpp \
		occlusion/occlusion.cpp \
		occlusion/frustrum_culling.cpp \
		occlusion/occlusion_culling.cpp \
		occlusion/culling_debug.cpp \
		occlusion/bitmask_culling.cpp \
		decal/decal.cpp \
		lighting/allocate_map.cpp \
		lighting/bake_lighting.cpp \
		lighting/calculate_lightmap.cpp \
		lighting/calculate_texture.cpp \
		lighting/point_light.cpp \
		lighting/fragment_light.cpp \
		lighting/rasterize_lighting/render_zbuffer.cpp \
		lighting/rasterize_lighting/rasterize_zbuffer.cpp \
		lighting/rasterize_lighting/rasterize_triangle_shadow.cpp \
		lighting/rasterize_lighting/rasterize_texture.cpp \
		lighting/rasterize_lighting/rasterize_light_utils.cpp \
		thread/thread.cpp \
		thread/thread_lighting.cpp \
		navigation/pathfind.cpp \
		navigation/navmesh.cpp \
		navigation/create_navmesh.cpp \
		navigation/nav_utils.cpp \
		navigation/connect_navmesh.cpp \
		render/rasterization/rasterize_triangle_wrap.cpp \
		render/rasterization/rasterize_triangle.cpp \
		render/rasterization/rasterize_triangle_transparent.cpp \
		render/draw.cpp \
		render/draw2.cpp \
		render/draw_rectangle.cpp \
		render/draw_shapes.cpp \
		render/draw_skybox.cpp \
		render/render_helper.cpp \
		render/surface_tools.cpp \
		render/colors.cpp \
		render/text.cpp \
		render/pixel_utils.cpp \
		render/render_dynamic_light.cpp \
		render/render_entity.cpp \
		render/renders_world_triangles_to_screen_triangles.cpp \
		render/render_buffer.cpp \
		render/init_render.cpp \
		render/gizmos/render_gizmo.cpp \
		render/gizmos/render_ball.cpp \
		render/gizmos/clip_quatline.cpp \
		render/transform/transform_triangle.cpp \
		render/transform/transform_quaternion.cpp \
		render/transform/clip_world_triangle_to_plane.cpp \
		render/transform/clip_screen_triangle_to_plane.cpp \
		render/transform/clip_screen_triangle_to_screen.cpp \
		render/transform/transform_bounds.cpp \
		world/entitycache.cpp \
		world/entitycache_2.cpp \
		world/entitycache_transparent.cpp \
		world/world.cpp \
		entity/components/comp_light.cpp \
		entity/components/comp_healthpack.cpp \
		entity/components/comp_player_spawn1.cpp \
		entity/components/comp_player_spawn2.cpp \
		entity/components/comp_gun_pickup1.cpp \
		entity/components/comp_gun_pickup2.cpp \
		entity/components/comp_auto_door.cpp \
		entity/components/comp_story_events1.cpp \
		entity/components/comp_story_events2.cpp \
		entity/components/comp_boutton1.cpp \
		entity/components/comp_boutton2.cpp #.ENDSRC. CREATECOMPONENT SCRIPT DEPENDS ON THIS SO DONT REMOVE
VECTORSRCFILES= conversions.cpp \
		conversions2.cpp \
		matrix_functions3.cpp \
		matrix_functions2.cpp \
		matrix_functions.cpp \
		matrix_rotations.cpp \
		matrix_shorthands.cpp \
		matrix_transform.cpp \
		point_complex2.cpp \
		point_complex.cpp \
		point_elementary.cpp \
		point_float_ops.cpp \
		point_more.cpp \
		point_shorthands.cpp \
		quaternions.cpp \
		quaternion_shorthands.cpp \
		vector2_clamp.cpp \
		vector2_complex2.cpp \
		vector2_complex3.cpp \
		vector2_complex.cpp \
		vector2_elementary.cpp \
		vector2_more.cpp \
		vector2_shorthands.cpp \
		vector2_round.cpp \
		vector3_cmp.cpp \
		vector3_complex2.cpp \
		vector3_complex3.cpp \
		vector3_complex.cpp \
		vector3_elementary.cpp \
		vector3_more.cpp \
		vector3_shorthands.cpp \
		vector3_shorthands2.cpp \
		barycentric.cpp \
		triangle_functions.cpp \
		triangle_functions2.cpp
VECTORSRC= $(addprefix src/vectors/,$(VECTORSRCFILES))
SRC= $(addprefix src/,$(SRCFILES))
SRC+= $(VECTORSRC)
OBJ= $(SRC:.cpp=.o)
DEPENDS= $(OBJ:.o=.d)

#Compilation stuff:
INCLUDE= -Isrc -Iinclude -Ilibft -I$(INSTALLED_LIBS_DIR)/include/SDL2/ \
			-I$(INSTALLED_LIBS_DIR)/include/FMOD/ #$(LIBFT)
CXX= g++
CXXFLAGS= $(INCLUDE) -g -MMD# -finline-functions -O3
LDFLAGS = -Wl,-rpath $(INSTALLED_LIBS_DIR)/lib

UNAME= $(shell uname)
ifeq ($(UNAME), Darwin)
override CXXFLAGS += '-D GL_SILENCE_DEPRECATION'
LIBS= $(LIBFT) -lm -framework OpenGL -L$(INSTALLED_LIBS_DIR)/lib -lSDL2 -lSDL2_ttf -L$(INSTALLED_LIBS_DIR)/lib -lfmod -lfmodL
AUTOGEN =
else ifeq ($(UNAME), Linux)
LIBS =  $(LIBFT) -lm -lGL -L$(INSTALLED_LIBS_DIR)/lib -lSDL2 -lSDL2_ttf -L$(INSTALLED_LIBS_DIR)/lib -lfmod -lfmodL -lpthread
AUTOGEN = ./autogen.sh &&
else
warning:
	@echo "Compilation for platform $(UNAME) not supported."
	exit 1
endif

#multi:
#	$(MAKE) -j6 all

all: $(SDL2) $(FREETYPE) $(SDL2_TTF) $(FMOD) $(LIBFT) $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(INCLUDE) $(LIBS) $(LDFLAGS)

$(OBJ): Makefile include/*.h

clean:
	rm -f $(OBJ)
	rm -f *.o

fclean: clean
	rm -f doom-nukem

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
