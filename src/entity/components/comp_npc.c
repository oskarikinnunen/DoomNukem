#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void assign_npc_type(t_npc *npc)
{
	if (!npc)
		return;
	npc->func_update = civilian_update;
	npc->func_movement = civilian_movement;
	npc->func_action = civilian_action;
	npc->func_anim = civilian_anim;
	npc->func_take_damage = civilian_take_damage;
	return ;
	if (npc->npc_type == nt_default)
	{
		npc->func_update = NULL;
		npc->func_movement = NULL;
		npc->func_action = NULL;
		npc->func_anim = NULL;
		npc->func_take_damage = NULL;
	}
	else if (npc->npc_type == nt_civilian)
	{
		npc->func_update = civilian_update;
		npc->func_movement = civilian_movement;
		npc->func_action = civilian_action;
		npc->func_anim = civilian_anim;
		npc->func_take_damage = civilian_take_damage;
	}
	if (npc->npc_type == nt_enemy)
	{
		npc->func_update = enemy_update;
		npc->func_movement = enemy_movement;
		npc->func_action = enemy_action;
		npc->func_anim = enemy_anim;
		npc->func_take_damage = enemy_take_damage;
	}
	npc->npc_type_changed = false;
}

//remember audio.sounds.c has a return remove it.
static void	comp_npc_update(t_entity *entity, t_world *world)
{
	if (!entity->animation.active)
		start_anim(&entity->animation, anim_forwards);
	else
		update_anim(&entity->animation, world->clock.delta);
		//entity_start_anim(entity, "anim");
	t_npc			*npc;

	npc = entity->component.data;
	if (npc == NULL)
		return;
	if (npc->npc_type_changed)
		assign_npc_type(npc);
	if (npc->func_update)
		npc->func_update(entity, world);
	if (npc->func_movement)
		npc->func_movement(entity, world);
	if (npc->func_action)
		npc->func_action(entity, world);
	if (npc->func_anim)
		npc->func_anim(entity, world);
}

static char *get_npc_type_name(int i)
{
	//Might be against norm to use enum to stack mem
	static char npc_types[nt_count][64] = \
	{
		"Default",
		"Civilian",
		"Enemy"
	};
	if (i < nt_count)
		return(npc_types[i]);
	return(NULL);
}

static void	comp_npc_gui_edit(t_entity *entity, t_autogui *gui, t_world *world)
{
	t_npc			*source;
	static bool		toggle_select;
	source = (t_npc *)entity->component.data;
	
	if (source == NULL)
	{
		gui_label("!NULL source", gui);
		return;
	}
	gui_starthorizontal(gui);
	gui_label("NPC: ", gui);
	gui_label(get_npc_type_name(source->npc_type), gui);
	gui_endhorizontal(gui);
	gui_labeled_bool("ANIM:", entity->animation.active, gui);
	gui_labeled_int("i:", entity->animation.frame, gui);

	if (gui_button("Move towards player", gui))
		source->aggroed = true;
	if (gui_highlighted_button_if("NPC Type", gui, toggle_select))
		toggle_select = !toggle_select;
	if (toggle_select)
	{
		int	i;

		i = 0;
		gui->offset.x = 15;
		while (i < nt_count)
		{
			if (gui_button(get_npc_type_name(i), gui))
			{
				/*
				gui->offset.x += 15;
				for (int index = 0; index < nt_count; index++)
				{
					gui_button(get_npc_type_name(index), gui);
				}*/
				source->npc_type = i;
				source->npc_type_changed = true;
				printf("test\n");
			}
			i++;
		}
		gui->offset.x = 0;
	}
}

static void	comp_npc_allocate(t_entity *entity, t_world *world)
{
	t_npc *source;

	entity->component.data = ft_memalloc(sizeof(t_npc));
	entity->component.data_size = sizeof(t_npc);
	source = entity->component.data;
	bzero(entity->component.data, sizeof(t_npc));
	source->movementspeed = 0.1f;
	start_anim(&entity->animation, anim_forwards);
	//entity_start_anim
	entity->animation.lastframe = 30;
	entity->animation.framerate = 30;
	entity->animation.loop = true;
	//source->path.start = get_nearest_target_node(world, entity->transform.position);
	assign_npc_type(source);
}

void	assign_component_npc(t_component *component)
{
	component->data_size = sizeof(t_npc);
	component_empty_function_pointers(component);
	component->func_allocate = comp_npc_allocate;
	component->func_update = comp_npc_update;
	component->func_gui_edit = comp_npc_gui_edit;
	assign_npc_type(component->data);
}
