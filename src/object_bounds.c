#include "doomnukem.h"

static void set_bounding_box(t_vector3 *max, t_vector3 *min, t_vector3 *vector, int count)
{
	t_vector3 v;
	int	i;

	max->x = -1000000000;
	max->y = -1000000000;
	max->z = -1000000000;
	min->x = 1000000000;
	min->y = 1000000000;
	min->z = 1000000000;
	i = 0;
	while (i < count)
	{
		v = vector[i];
		if (v.x < min->x)
			min->x = v.x;
		if (v.x > max->x)
			max->x = v.x;
		if (v.y < min->y)
			min->y = v.y;
		if (v.y > max->y)
			max->y = v.y;
		if (v.z < min->z)
			min->z = v.z;
		if (v.z > max->z)
			max->z = v.z;
		i++;
	}
}

static float get_box_sphere_radius(t_vector3 origin, t_vector3 max, t_vector3 min)
{
	float	min_radius;
	float	max_radius;

	min_radius = vector3_dist(origin, min);
	max_radius = vector3_dist(origin, max);
	if (max_radius > min_radius)
		return(max_radius);
	else
		return(min_radius);
}

void set_object_boundingbox(t_object *obj)
{
	t_vector3	max;
	t_vector3	min;
	set_bounding_box(&max, &min, obj->vertices, obj->vertice_count);
	obj->bounds.origin = vector3_lerp(max, min, 0.5f);
	obj->bounds.radius = get_box_sphere_radius(obj->bounds.origin, max, min);
	obj->bounds.box.v[0] = (t_vector3){.x = max.x, .y = max.y, min.z};
	obj->bounds.box.v[1] = (t_vector3){.x = max.x, .y = min.y, min.z};
	obj->bounds.box.v[2] = (t_vector3){.x = min.x, .y = min.y, min.z};
	obj->bounds.box.v[3] = (t_vector3){.x = min.x, .y = max.y, min.z};
	obj->bounds.box.v[4] = (t_vector3){.x = max.x, .y = max.y, max.z};
	obj->bounds.box.v[5] = (t_vector3){.x = max.x, .y = min.y, max.z};
	obj->bounds.box.v[6] = (t_vector3){.x = min.x, .y = min.y, max.z};
	obj->bounds.box.v[7] = (t_vector3){.x = min.x, .y = max.y, max.z};
}
