#include "doomnukem.h"

typedef	struct s_rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} t_rgb;

typedef struct s_color
{
	union cdata_u
	{
		t_rgb		rgb;
		uint32_t	color;
	} dat;
}	t_color;

static uint32_t	flip_channels(uint32_t clr)
{
	t_color		result;
	t_color		orig;
	orig.dat.color = clr;
	result.dat.rgb.r = orig.dat.rgb.b;
	result.dat.rgb.g = orig.dat.rgb.g;
	result.dat.rgb.b = orig.dat.rgb.r;
	result.dat.rgb.a = orig.dat.rgb.a;
	return (result.dat.color);
}

static float sign1 (t_point p1, t_point p2, t_point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (t_point pt, t_point v1, t_point v2, t_point v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign1(pt, v1, v2);
    d2 = sign1(pt, v2, v3);
    d3 = sign1(pt, v3, v1);

    has_neg = (d1 < 0.0f) || (d2 < 0.0f) || (d3 < 0.0f);
    has_pos = (d1 > 0.0f) || (d2 > 0.0f) || (d3 > 0.0f);

    return !(has_neg && has_pos);
}

static bool intersect_triangle(t_ray r, t_texture *t, t_vector3 a, t_vector3 b, t_vector3 c)
{
    t_vector3   e1, e2, n;

    e1 = vector3_sub(b, a);
    e2 = vector3_sub(c, a);
    n = vector3_crossproduct(e1, e2);

    float det, invdet;
    det = -vector3_dot(r.dir, n);
    invdet = 1.0f/det;
    t_vector3 ao = vector3_sub(r.origin, a);
    t_vector3 dao = vector3_crossproduct(ao, r.dir);
    t->u = vector3_dot(e2, dao) * invdet;
    t->v = -vector3_dot(e1, dao) * invdet;
    t->w = vector3_dot(ao, n) * invdet;
    return(fabs(det) >= 1e-6 && t->w >= 0.0f && t->u >= 0.0f && t->v >= 0.0f && (t->u + t->v) <= 1.0f);
}

static t_vector3 texcoord_to_loc(t_vector3 *v, t_vector2 *t, t_vector2 p)
{
    float       i;
    float       s;
    float       delta;

    t_vector3   r;
    i = 1.0f / ((t[1].x - t[0].x) * (t[2].y - t[0].y) - (t[1].y - t[0].y) * (t[2].x - t[0].x));
    s = i * ((t[2].y - t[0].y) * (p.x - t[0].x) - (t[2].x - t[0].x) * (p.y - t[0].y));
    delta = i * (-(t[1].y - t[0].y) * (p.x - t[0].x) + (t[1].x - t[0].x) * (p.y - t[0].y));
    r.x = v[0].x + s * (v[1].x - v[0].x) + delta * (v[2].x - v[0].x);
    r.y = v[0].y + s * (v[1].y - v[0].y) + delta * (v[2].y - v[0].y);
    r.z = v[0].z + s * (v[1].z - v[0].z) + delta * (v[2].z - v[0].z);
    return r;
}

void calculate_pointlight(t_pointlight *pointlight, t_world *world, t_render *render)
{
    int i = 0;
    int found = 0;
    int index = 0;
	int j;
	t_lighting	l;
	t_entity *ent;

    l.triangles = malloc(sizeof(t_triangle_polygon *) * world->entitycache.existing_entitycount);
    l.entities = malloc(sizeof(t_entity *) * world->entitycache.existing_entitycount);
    while (found < world->entitycache.existing_entitycount)
    {
        ent = &world->entitycache.entities[i];
        if (ent->status != es_free)
        {
            if (ent->status == es_active)
            {
				l.triangles[index] = malloc(sizeof(t_triangle_polygon) * ent->obj->face_count);
				l.entities[index] = ent;
				j = 0;
				while (j < ent->obj->face_count)
				{
					for (int e = 0; e < 3; e++)
					{
						t_quaternion temp;
						temp.v = ent->obj->vertices[ent->obj->faces[j].v_indices[e] - 1];
						temp = transformed_vector3(ent->transform, temp.v);
						temp = quaternion_mul_matrix(render->camera.matworld, temp);
						l.triangles[index][j].p3[e] = temp.v;
						l.triangles[index][j].uv[e] = ent->obj->uvs[ent->obj->faces[j].uv_indices[e] - 1];

					}
					j++;
				}	
				index++;
            }
            found++;
        }
        i++;
    }
	l.entities_count = index;
	t_lightmap *lightmap;
	i = 0;
	while (i < index)
	{
		ent = l.entities[i];
		j = 0;
		while (j < ent->obj->face_count)
		{
			t_point		tri[3];
			if (j == 0 || ent->obj->faces[j].materialindex != ent->obj->faces[j - 1].materialindex)
			{
				lightmap = &ent->lightmap[ent->obj->faces[j].materialindex];
				//l.map = malloc(sizeof(bool) * lightmap->size.x * lightmap->size.y);
				//bzero(l.map, sizeof(bool) * lightmap->size.x * lightmap->size.y);
			}
			for (int e = 0; e < 3; e++)
			{
				t_point uv;
				uv.x = l.triangles[i][j].uv[e].x * (ent->obj->materials[ent->obj->faces[j].materialindex].img->size.x - 1);
				uv.y = l.triangles[i][j].uv[e].y * (ent->obj->materials[ent->obj->faces[j].materialindex].img->size.y - 1);
				tri[e].x = uv.x;
				tri[e].y = uv.y;

				l.triangles[i][j].uv[e].x = uv.x / (lightmap->size.x);
				l.triangles[i][j].uv[e].y = uv.y / (lightmap->size.y);
			}
			l.lightmap = lightmap;
			l.pointlight = pointlight;
			t_triangle_polygon test;
			memcpy(test.p2, tri, sizeof(t_point) * 3);
			memcpy(test.uv, l.triangles[i][j].uv, sizeof(t_vector2) * 3);
			memcpy(test.p3, l.triangles[i][j].p3, sizeof(t_vector3) * 3);
			render_triangle_uv(l, test);
			//if (j + 1 == ent->obj->face_count || ent->obj->faces[j].materialindex != ent->obj->faces[j + 1].materialindex)
			//	free(l.map);
			j++;
		}
		i++;
	}
	i = 0;
	while (i < index)
	{
		free(l.triangles[i]);
		i++;
	}
	free(l.triangles);
	free(l.entities);
}
