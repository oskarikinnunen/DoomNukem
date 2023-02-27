#include "doomnukem.h"

uint32_t	update_pixel_brightness(uint8_t light, uint32_t clr)
{
    Uint32 alpha = clr & 0xFF000000;
    Uint32 red = ((clr & 0x00FF0000) * light) >> 8;
    Uint32 green = ((clr & 0x0000FF00) * light) >> 8;
    Uint32 blue = ((clr & 0x000000FF) * light) >> 8;

    clr = alpha | (red & 0x00FF0000) | (green & 0x0000FF00) | (blue & 0x000000FF);
    return(clr);
}

inline t_step	make_slope(int ax, int bx, float delta)
{
	t_step temp;

	temp.location = ax;
	temp.step = (bx - ax) * delta;
	return(temp);
}

uint32_t	get_lighting_for_pixel(t_entitycache *cache, t_vector3 location)
{
    int             j;
    int				i;
	int				found;
	t_entity		*ent;
    uint32_t        light_amount;
    t_quaternion    q;

	light_amount = 0;
	i = 0;
	found = 0;
	while (found < cache->existing_entitycount
		&& i < cache->alloc_count)
	{
		ent = &cache->entities[i];
		if (ent->status != es_free)
		{
			if	(ent->status == es_active && ent->component.type == COMP_LIGHT)
			{
                t_light *light = ent->component.data;
				t_vector3 dir;
                dir = vector3_add(ent->transform.position, light->origin);
                dir = vector3_sub(location, dir);
                dir = vector3_normalise(dir);

                j = 0;
                float temp = dir.x;
                if (fabsf(dir.y) > fabsf(temp))
                {
                    temp = dir.y;
                    j = 1;
                }
                if (fabsf(dir.z) > fabsf(temp))
                {
					temp = dir.z;
					j = 2;
                }
                if (temp < 0.0f)
                    j += 3;
                t_point     p;
                t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
                q = vector3_to_quaternion(location);
                q = quaternion_mul_matrix(light->cubemap.cameras[j].matview, q);
                q = quaternion_mul_matrix(light->cubemap.cameras[j].matproj, q);
                q.v = vector3_div(q.v, q.w);
                q.v = vector3_negative(q.v);
                q.v = vector3_add(q.v, voffsetview);
                p.x = q.v.x * (light->cubemap.resolution.x * 0.5f);
                p.y = q.v.y * (light->cubemap.resolution.y * 0.5f);

                p.x = ft_clamp(p.x, 0, light->cubemap.resolution.x);
                p.y = ft_clamp(p.y, 0, light->cubemap.resolution.y);

                int test = p.y * light->cubemap.resolution.x + p.x;
                if (1.0f / q.w >= light->cubemap.shadowmaps[j][test] * 0.98f && q.w < light->radius)
                {
                    float delta = 1.0f - (q.w / light->radius);
                    light_amount += delta * 255.0f;
                }
			}
			found++;
		}
		i++;
	}
    return(light_amount);
}

t_vector3 texcoord_to_loc(t_vector3 ws[3], t_vector2 uv[3], t_vector2 p)
{
	float       i;
	float       s;
	float       delta;

	t_vector3   r;
	i = 1.0f / ((uv[1].x - uv[0].x) * (uv[2].y - uv[0].y) - (uv[1].y - uv[0].y) * (uv[2].x - uv[0].x));
	s = i * ((uv[2].y - uv[0].y) * (p.x - uv[0].x) - (uv[2].x - uv[0].x) * (p.y - uv[0].y));
	delta = i * (-(uv[1].y - uv[0].y) * (p.x - uv[0].x) + (uv[1].x - uv[0].x) * (p.y - uv[0].y));
	r.x = ws[0].x + s * (ws[1].x - ws[0].x) + delta * (ws[2].x - ws[0].x);
	r.y = ws[0].y + s * (ws[1].y - ws[0].y) + delta * (ws[2].y - ws[0].y);
	r.z = ws[0].z + s * (ws[1].z - ws[0].z) + delta * (ws[2].z - ws[0].z);
	return r;
}
