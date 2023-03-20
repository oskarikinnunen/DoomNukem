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

static void make_uv(t_world *world, t_decal *decal)
{
	t_camera camera;

}

static void entity_triangles(t_entity *entity, t_decal *decal)
{
	t_triangle out_tri[4];
	int i;
	t_point a;
	t_point b;
	t_triangle *temp_buffer;
	int e = 0;

	temp_buffer = malloc(sizeof(t_triangle) * entity->obj->face_count * 4);
	if (temp_buffer == NULL)
		doomlog(LOG_FATAL, "Malloc failed in entity_triangles.c");
	i = 0;
	while (i < entity->obj->face_count)
	{
		a.y = clip_triangle_against_plane(decal->frustrum[0], (t_vector3){0.0f, 0.0f, 1.0f}, entity->world_triangles[i], out_tri);
		a.x = 0;
		while (a.x < a.y)
		{
			b.y = clip_triangle_against_plane(decal->frustrum[1], (t_vector3){0.0f, 0.0f, 1.0f}, out_tri[a.x], &out_tri[2]);
			b.x = 0;
			while (b.x < b.y)
			{
				temp_buffer[e++] = out_tri[2 + b.x];
				b.x++;
			}
			a.x++;
		}
		i++;
	}
	t_triangle *temp;
	temp = decal->entity->world_triangles;
	decal->entity->world_triangles = malloc((decal->entity->obj->face_count + e) * sizeof(t_triangle));
	if (decal->entity->world_triangles == NULL)
		doomlog(LOG_FATAL, "Malloc failed in decal.c");
	ft_memcpy(decal->entity->world_triangles, temp, sizeof(t_triangle) * decal->entity->obj->face_count);
	ft_memcpy(&decal->entity->world_triangles[decal->entity->obj->face_count], temp_buffer, sizeof(t_triangle) * e);
	decal->entity->obj->face_count += e;
	free(temp);
	free(temp_buffer);
}

void get_triangles(t_world *world, t_decal *decal)
{
	int			i;
	int			found;
	t_entity	*ent;
	
	i = 0;
	found = 0;
	while (found < world->entitycache.existing_entitycount)
	{
		ent = &world->entitycache.entities[i];
		if (ent->status != es_free)
		{
			if (ent->status == es_active && !ent->hidden)
			{
				if (vector3_dist(decal->position, get_entity_world_position(ent)) - ent->obj->bounds.radius < decal->size)
					entity_triangles(ent, decal);
			}
			found++;
		}
		i++;
	}
	free(decal->entity->obj->faces);
	decal->entity->obj->faces = malloc(sizeof(t_face) * decal->entity->obj->face_count);
	ft_bzero(decal->entity->obj->faces, sizeof(t_face) * decal->entity->obj->face_count);
	decal->entity->matworld = make_transform_matrix(decal->entity->transform);
}

void decal(struct s_world *world, t_decal decal)
{
	t_object	*obj;
	t_entity	*entity;
	t_vector3	normal;

	obj = object_plane(world->sdl);
	obj->materials->img = get_image_by_name(*world->sdl, "car_red.cng");
	decal.entity = spawn_entity(world, obj);
	decal.entity->transform.position = decal.position;
	decal.size = 150;
	decal.frustrum[0] = vector3_add(decal.position, vector3_mul(decal.normal, decal.size));
	decal.frustrum[1] = vector3_add(decal.position, vector3_mul(vector3_negative(decal.normal), decal.size));
	get_triangles(world, &decal);
	make_uv(world, &decal);
}
