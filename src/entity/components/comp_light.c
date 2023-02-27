/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlaine <vlaine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 18:14:04 by okinnune          #+#    #+#             */
/*   Updated: 2023/02/27 16:18:44 by vlaine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

/* Called once per frame, use this to update your entitys state */
void	comp_light_update(t_entity *entity, t_world *world)
{
	t_light	*light;

	light = entity->component.data;
	if (light == NULL)
		return ;
	//if playmode
	//
}


void	highlight_bound(t_entity *ent, t_world *world)
{
	/*if (ft_strequ(ent->obj->name, "meshtri"))
	{
		printf("rendering floor bound with radius %f \n", ent->obj->bounds.radius);
		render_ball(world->sdl, ent->obj->bounds.origin, ent->obj->bounds.radius, CLR_RED);
	}*/
}

//make this take one shadowmap and scale it to fit screen
static void	draw_shadowmap(t_light *light, t_cubemap *cubemap, t_sdlcontext *sdl)
{
	int x;
	int y;
	int	width;
	t_point	size;
	float	w;

	size.x = cubemap->resolution.x / 8;
	size.y = cubemap->resolution.y / 8;
	width = sdl->window_w - size.x;
	y = 0;
	while (y < size.y)
	{	
		x = 0;
		while (x < size.x)
		{
			for (int i = 0; i < 6; i++)
			{
				float tempx = (float)x/(float)size.x;
				float tempy = (float)y/(float)size.y;
				tempx = tempx * cubemap->resolution.x;
				tempy = tempy * cubemap->resolution.y;

				//printf("x, y %f %f\n", tempx, tempy);
				float w = 1.0f / cubemap->shadowmaps[i][(int)(tempy * cubemap->resolution.x + tempx)];

				uint32_t clr = INT_MAX;
				w = (light->radius - w) / light->radius;
				w = ft_flerp(0.2f, 0.8f, w);
				uint8_t p = ft_clamp(w * 255.0f, 0, 255);
				Uint32 alpha = clr & 0xFF000000;
				Uint32 red = ((clr & 0x00FF0000) * p) >> 8;
				Uint32 green = ((clr & 0x0000FF00) * p) >> 8;
				Uint32 blue = ((clr & 0x000000FF) * p) >> 8;
				clr = flip_channels(alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF));
				
				((uint32_t *)sdl->surface->pixels)[((y + sdl->window_h - size.y) * (sdl->window_w)) + x + (i * size.x)] = clr;
			}
			x++;
		}
		y++;
	}
}

/* Called once per frame after the 3D world has been drawn, use this to draw gizmos/rays/whatever*/
void	comp_light_ui_update(t_entity *entity, t_world *world)
{
	t_light			*light;
	static t_img	*icon;

	light = entity->component.data;
	if (light == NULL)
		return ;
	if (icon == NULL)
		icon = get_image_by_name(*world->sdl, "light_icon.cng");
	if (!entity->occlusion.is_occluded)
	{
		t_vector3	lpos;

		lpos = transformed_vector3(entity->transform, light->origin).v;
		draw_worldspace_icon(lpos, icon, world);
		//draw_worldspace_i_icon(entity, icon, world);
	}
	for_all_active_entities(world, highlight_bound);
	if (light->cm_state != cm_default)
	{
		draw_shadowmap(light, &light->cubemap, world->sdl);
	}
}

/*
	calculate_pointlight(t_entity *light_ent, t_world *world)
	{
		foreach active entity
		if (inrange)
		{
			for (triangles)
				calculate tri poly
				rasterize uv tri(t_entity *, tripoly *, world *)
		}

	}

*/

/* Used to edit component values */
void	comp_light_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_light		*light;
	static bool	toggle_select;
	char		cubemap_nb[2];
	int			i;
	

	light = entity->component.data;
	if (light == NULL)
		return;
	cubemap_nb[1] = '\0';
	gui_labeled_float_slider("Radius: ", &light->radius, 0.1f, gui);
	gui_labeled_vector3_slider("Offset:", &light->origin, 0.1f, gui);
	render_ball(world->sdl, entity->transform.position, light->radius, AMBER_3);
	if (gui_highlighted_button_if("CubeMap", gui, toggle_select))
		toggle_select = !toggle_select;
	if (toggle_select)
	{
		i = 0;
		gui->offset.x = 15;
		while (i < 6)
		{
			cubemap_nb[0] = i + '1';
			if (gui_button(cubemap_nb, gui))
			{
				for (int y = 0; y < light->cubemap.resolution.y && 0; y++)
				{
					for (int x = 0; x < light->cubemap.resolution.x; x++)
					{
						printf("%f ", light->cubemap.shadowmaps[i][(int)(y * light->cubemap.resolution.x + x)]);
					}
					printf("\n");
				}
				light->cm_state = i + 1;
			}
			i++;
		}
		gui->offset.x = 0;
	}
	else
		light->cm_state = cm_default;
	if (light->cm_state != cm_default)
	{
		render_gizmo3d(world->sdl, vector3_add(entity->transform.position, light->origin), 50, CLR_RED);
		render_ray3D(world->sdl, vector3_add(entity->transform.position, light->origin), vector3_add(vector3_mul(light->cubemap.cameras[light->cm_state - 1].lookdir, 25.0f), vector3_add(entity->transform.position, light->origin)), INT_MAX);
	}
	t_mat4x4 matproj = matrix_makeprojection(90.0f, light->cubemap.resolution.y / light->cubemap.resolution.x, 2.0f, 1000.0f);
	i = 0;
	while (i < 6)
	{
		light->cubemap.cameras[i].position = vector3_add(entity->transform.position, light->origin); 
		light->cubemap.cameras[i].matproj = matproj;
		render_start(&light->cubemap.cameras[i]);
		i++;
	}
}
/*	This is called during load_world, use only if your component
	NEEDS to gather assets (sounds etc.) at this time.
*/
void	comp_light_loadassets(t_entity *entity, t_world *world)
{
	t_light	*light;
	light = entity->component.data;
}

/*	Set default values of your component here.
	This will be called when editor assigns this component type to an entity,
	or during entity_set_component()
*/
void	comp_light_allocate(t_entity *entity, t_world *world)
{
	t_mat4x4	matproj;
	t_light		*light;
	int			i;

	entity->component.data = ft_memalloc(sizeof(t_light));
	entity->component.data_size = sizeof(t_light);
	light = (t_light *)entity->component.data;
	//light->cubemap.shadowmaps = (float **)malloc(sizeof(float *) * 6);
	light->radius = 500.0f;
	light->cubemap.cameras[0].lookdir = (t_vector3){.x = 1.0f};
	light->cubemap.cameras[1].lookdir = (t_vector3){.y = 1.0f};
	light->cubemap.cameras[2].lookdir = (t_vector3){.x = 0.01f, .z = 0.99f};
	light->cubemap.cameras[3].lookdir = (t_vector3){.x = -1.0f};
	light->cubemap.cameras[4].lookdir = (t_vector3){.y = -1.0f};
	light->cubemap.cameras[5].lookdir = (t_vector3){.x = -0.01f, .z = -0.99f};
	light->cubemap.resolution.x = 10000;
	light->cubemap.resolution.y = 10000;
	light->ignoreself = false;
	matproj = matrix_makeprojection(90.0f, light->cubemap.resolution.y / light->cubemap.resolution.x, 2.0f, 1000.0f);
	i = 0;
	while (i < 6)
	{
		light->cubemap.shadowmaps[i] = (float *)malloc(sizeof(float) * (light->cubemap.resolution.x * light->cubemap.resolution.y));
		light->cubemap.cameras[i].position = vector3_add(entity->transform.position, light->origin); 
		light->cubemap.cameras[i].matproj = matproj;
		render_start(&light->cubemap.cameras[i]);
		i++;
	}
}

/*	Internal function that's used to link this components behaviour
	TODO: better comment, lol
*/
void	assign_component_light(t_component *component)
{
	component->data_size = sizeof(t_light);
	component->func_allocate = comp_light_allocate;
	component->func_update = comp_light_update;
	//component->func_gui_edit = comp_light_gui_edit;
	component->func_gui_edit = comp_light_gui_edit;
	component->func_ui_update = comp_light_ui_update;
	component->func_loadassets = comp_light_loadassets;
}