#include "doomnukem.h"

t_vector3	get_entity_world_position(t_entity *entity)
{
	t_transform	 *node;
	t_vector3	position;

	position = vector3_add(entity->transform.position, entity->obj->bounds.origin);
	node = entity->transform.parent;
	while (node)
	{
		position = vector3_add(position, node->position);
		node = node->parent;
	}
	return(position);
}
