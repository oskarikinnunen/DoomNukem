#include "doomnukem.h"

/*
make a plane
get hit location
get img and size in world
add offset to plane to be a bit more on players side so that zbuffer isnt a problem
add transparent tag

*/
t_quaternion quaternion_axis_angle(t_vector3 axis, float angle)
{
	t_quaternion q;

	q.v = vector3_mul(axis, sinf(angle / 2.0f));
	q.w = cosf(angle / 2.0f);
	return(q);
}

/*

	// Calculate new forward direction
	t_vector3 newForward = vector3_sub(target, pos);
	newForward = vector3_normalise(newForward);

	// Calculate new Up direction
	t_vector3 a = vector3_mul(newForward, vector3_dot(up, newForward));
	t_vector3 newUp = vector3_sub(up, a);
	newUp = vector3_normalise(newUp);

	// New Right direction is easy, its just cross product
	t_vector3 newRight = vector3_crossproduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix
	void decal(struct s_world *world, t_decal decal)
{
	t_object	*obj;
	t_entity	*entity;
	t_vector3	normal;

	normal = (t_vector3){-decal.normal.y, decal.normal.x, decal.normal.z};
	obj = object_plane(world->sdl);
	obj->materials->img = get_image_by_name(*world->sdl, "car_red.cng");
	entity = spawn_entity(world, obj);

	t_vector3 a = vector3_mul(decal.normal, vector3_dot((t_vector3){0.0f, 0.0f, 1.0f}, decal.normal));
	t_vector3 newUp = vector3_sub((t_vector3){0.0f, 0.0f, 1.0f}, a);
	newUp = vector3_normalise(newUp);

	t_vector3 rotationaxis = vector3_crossproduct(newUp, decal.normal);
	rotationaxis = vector3_normalise(rotationaxis);

	float dot = vector3_dot(newUp, decal.normal);
	float rotationgangle = acosf(dot);

	t_quaternion q = quaternion_axis_angle(rotationaxis, rotationgangle);
	entity->transform.rotation = q.v;
	entity->transform.position = decal.position;
	//entity->transform.rotation = Fquat.v;
	printf("spaned\n");
}
plane->vertices[0] = (t_vector3){0.0f, 0.0f, 0.0f};
	plane->vertices[1] = (t_vector3){10.0f, 0.0f, 0.0f};
	plane->vertices[2] = (t_vector3){0.0f, 0.0f, 10.0f};
	plane->vertices[3] = (t_vector3){10.0f, 0.0f, 10.0f};
(t_vector3){0.0f, 0.0f, 1.0f}
	*/
t_vector3 rotationmatrixtoeuler(t_mat4x4 r)
{
	t_vector3	v;
	float sy = sqrtf(r.m[0][0] * r.m[0][0] + r.m[1][0] * r.m[1][0]);

	if (!(sy < 1e-6))
	{
		v.x = atan2f(r.m[2][1], r.m[2][2]);
		v.y = atan2f(-r.m[2][0], sy);
		v.z = atan2f(r.m[1][0], r.m[0][0]);
	}
	else
	{
		v.x = atan2f(-r.m[1][2], r.m[1][1]);
		v.y = atan2f(-r.m[2][0], sy);
		v.z = 0;
	}
	return(v);
}

t_vector3 degreeturn(t_vector3 v)
{
	float temp;
	temp = v.x;
	v.x = -v.y;
	v.y = temp;
	return(v);
}

void decal(struct s_world *world, t_decal decal)
{
	t_object	*obj;
	t_entity	*entity;
	t_vector3	normal;

	obj = object_plane(world->sdl);
	obj->materials->img = get_image_by_name(*world->sdl, "car_red.cng");
	entity = spawn_entity(world, obj);

	obj->vertices[1] = vector3_mul((t_vector3){.x = decal.normal.x, .y = decal.normal.y}, 10.0f);
	obj->vertices[2] = vector3_mul((t_vector3){.z = decal.normal.z}, 10.0f);
	obj->vertices[3] = vector3_mul((t_vector3){.x = decal.normal.x, .z = decal.normal.z}, 10.0f);
	entity->transform.position = decal.position;
}