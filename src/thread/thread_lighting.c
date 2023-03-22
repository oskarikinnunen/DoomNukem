#include "doomnukem.h"

void	set_light_thread_struct(
	t_thread *thread, struct s_entity *ent, struct s_world *world)
{
	t_test	*ptr;

	if (ent->status != es_active && ent->component.type != COMP_LIGHT)
		return ;
	ptr = &(((t_test *)thread->structs)[thread->count++]);
	ptr->entity = ent;
	ptr->world = world;
	if (thread->count == THREAD)
		multithread_start(thread);
}

void	set_lighting_texture_struct(t_thread *thread, struct s_entity *ent)
{
	if	(ent->status == es_active && ent->map != NULL)
	{
		(((t_entity *)thread->structs)[thread->count++]) = *ent;
		if (thread->count == THREAD)
			multithread_start(thread);
	}
}
