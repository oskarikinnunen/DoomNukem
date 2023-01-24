#include "entity.h"
#include "doomnukem.h"
#include "editor_tools.h"

static void assign_npc_type(t_npc *source)
{
	if (!source)
		return;
	if (source->npc_type == nt_default)
	{
		source->func_update = NULL;
		source->func_movement = NULL;
		source->func_action = NULL;
		source->func_anim = NULL;
		source->func_take_damage = NULL;
	}
	else if (source->npc_type == nt_civilian)
	{
		source->func_update = civilian_update;
		source->func_movement = civilian_movement;
		source->func_action = civilian_action;
		source->func_anim = civilian_anim;
		source->func_take_damage = civilian_take_damage;
	}
	else if (source->npc_type == nt_enemy)
	{
		source->func_update = enemy_update;
		source->func_movement = enemy_movement;
		source->func_action = enemy_action;
		source->func_anim = enemy_anim;
		source->func_take_damage = enemy_take_damage;
	}
	source->npc_type_changed = false;
}

//remember audio.sounds.c has a return remove it.
static void	comp_npc_update(t_entity *entity, t_world *world)
{
	t_npc			*source;

	source = entity->component.data;
	if (source == NULL)
		return;
	if (source->npc_type_changed)
		assign_npc_type(source);
	if (source->func_update)
		source->func_update(entity, world);
	if (source->func_movement)
		source->func_movement(entity, world);
	if (source->func_action)
		source->func_action(entity, world);
	if (source->func_anim)
		source->func_anim(entity, world);
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

static void	comp_npc_gui_edit(t_component *component, t_autogui *gui)
{
	t_npc			*source;
	static bool		toggle_select;
	source = component->data;
	
	if (source == NULL)
	{
		gui_label("!NULL source", gui);
		return;
	}
	gui_starthorizontal(gui);
	gui_label("NPC: ", gui);
	gui_label(get_npc_type_name(source->npc_type), gui);
	gui_endhorizontal(gui);

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
	source->path.start = get_nearest_target_node(world, entity->transform.position);
	assign_npc_type(source);
}

void	assign_component_npc(t_component *component)
{
	component->data_size = sizeof(t_npc);
	component->func_allocate = comp_npc_allocate;
	component->func_update = comp_npc_update;
	component->func_gui_edit = comp_npc_gui_edit;
	assign_npc_type(component->data);
}
