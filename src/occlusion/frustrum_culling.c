#include "doomnukem.h"

bool is_entity_frustrum_culled(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
	t_camera		c;
	t_quaternion	origin;
	float			radius;
	float			dist;

	c = render->camera;
	origin = transformed_vector3(entity->transform, entity->obj->bounds.origin);
	float max = 0.0f;
	if (max < entity->transform.scale.x)
		max = entity->transform.scale.x;
	if (max < entity->transform.scale.y)
		max = entity->transform.scale.y;
	if (max < entity->transform.scale.z)
		max = entity->transform.scale.z;
	radius *= max;
	origin = quaternion_mul_matrix(c.matworld, origin);
	origin = quaternion_mul_matrix(c.matview, origin);
	dist = vector3_fdist_to_plane(origin.v, (t_vector3){.z = 1.0f}, (t_vector3){.z = 0.1f});
	if (dist < 0.0f)
	{
		if (dist + radius < 0.0f)
			return(true);
		else
			return(false);
	}
	origin = quaternion_mul_matrix(c.matproj, origin);
	origin.v = vector3_div(origin.v, origin.w);
	origin.v = vector3_negative(origin.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	origin.v = vector3_add(origin.v, voffsetview);

	t_vector2	pos;
	pos.x = origin.v.x * 0.5f * (float)sdl.window_w;
	pos.y = origin.v.y * 0.5f * (float)sdl.window_h;
	if (
	vector2_fdist_to_plane(pos, (t_vector2){0.0f, 1.0f}, (t_vector2){0.0f, 0.0f}) + radius < 0.0f || 
	vector2_fdist_to_plane(pos, (t_vector2){0.0f, -1.0f}, (t_vector2){0.0f, (float)(sdl.window_h) - 1.0f}) + radius < 0.0f ||
	vector2_fdist_to_plane(pos, (t_vector2){1.0f, 0.0f}, (t_vector2){0.0f, 0.0f}) + radius < 0.0f ||
	vector2_fdist_to_plane(pos, (t_vector2){-1.0f, 0.0f}, (t_vector2){(float)(sdl.window_w) - 1.0f, 0.0f}) + radius < 0.0f)
		return(true);
	return (false);
}
