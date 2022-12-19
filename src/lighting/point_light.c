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

typedef struct s_ray
{
    t_vector3 origin;
    t_vector3 dir; //direction vector
}   t_ray;

bool intersect_triangle(t_ray r, t_texture *t, t_vector3 a, t_vector3 b, t_vector3 c)
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
    return(fabs(det) >= 1e-6 && t->w >= 0.00000f && t->u >= 0.00000f && t->v >= 0.00000f && (t->u + t->v) <= 1.00000f);
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

static bool SameSide(t_vector3 A, t_vector3 B, t_vector3 C, t_vector3 D, t_vector3 p)
{
    t_vector3 normal = vector3_crossproduct(vector3_sub(B, A), vector3_sub(C, A));
    float dotD = vector3_dot(normal, vector3_sub(D, A));
    float dotP = vector3_dot(normal, vector3_sub(p, A));
    return signbit(dotD) == signbit(dotP);
}

static bool LineIntersectTri(t_vector3 A, t_vector3 B, t_vector3 C, t_vector3 O, t_vector3 D)
{
    return SameSide(A, B, C, O, vector3_add(O, D)) &&
           SameSide(B, C, O, A, vector3_add(O, D)) &&
           SameSide(C, O, A, B, vector3_add(O, D)) &&
           SameSide(O, A, B, C, vector3_add(O, D));               
}

static void ray_uv(t_triangle triangle, t_render *render, int id, int t_index)
{
    t_quaternion	*p;
	t_texture		*t;
	t_point			max;
	t_point			min;
	t_point		    lt[3];
    t_vector2       tes[3];
    t_vector3   arr[3];
	int				x;
	int				y;

	p = triangle.p;
	t = triangle.t;
	max.x = -100000;
	max.y = -100000;
	min.x = 100000;
	min.y = 100000;
	for(int e = 0; e < 3; e++)
	{
		lt[e].x = t[e].u * (render->img->size.x - 1);
		lt[e].y = t[e].v * (render->img->size.y - 1);
        tes[e].x = lt[e].x / (float)render->lightmap->size.x;
        tes[e].y = lt[e].y / (float)render->lightmap->size.y;
        arr[e] = p[e].v;
		if (lt[e].x > max.x)
			max.x = lt[e].x;
		if (lt[e].y > max.y)
			max.y = lt[e].y;
		if (lt[e].x < min.x)
			min.x = lt[e].x;
		if (lt[e].y < min.y)
			min.y = lt[e].y;
	}
    int count = 0;
	y = min.y;
    bool tst2 = false;
	while (y < max.y)
	{
		x = min.x;
        bool first = false;
		while (x < max.x)
		{
			if (PointInTriangle((t_point){x, y}, (t_point){tes[0].x * render->lightmap->size.x, tes[0].y * render->lightmap->size.y}, (t_point){tes[1].x * render->lightmap->size.x, tes[1].y * render->lightmap->size.y}, (t_point){tes[2].x * render->lightmap->size.x, tes[2].y * render->lightmap->size.y}))
			{
               /* if (first == false)
                {
                    first = true;
                    x++;
                    continue;
                }
                else if (x + 1 < max.x && PointInTriangle((t_point){x + 1, y}, lt[0], lt[1], lt[2]) == false)
                {
                    x++;
                    continue;
                }*/
                
                t_ray       ray;
                
                ray.origin = texcoord_to_loc(arr, tes, (t_vector2){x/(float)render->lightmap->size.x, y/(float)render->lightmap->size.y});
                ray.dir = vector3_sub((t_vector3){500.0f, 500.0f, 250.0f}, ray.origin);
                int i = 0;
                int found = 0;
                bool ol = false;
                while (found < render->world->entitycache.existing_entitycount)
                {
                    t_entity *ent = &render->world->entitycache.entities[i];
                    if (ent->status != es_free)
                    {
                        if (ent->status == es_active)
                        {
                            int index = 0;
                            while (index < ent->obj->face_count)
                            {
                                t_vector3 a, b, c;
                                t_quaternion temp;
                                temp.v = ent->obj->vertices[ent->obj->faces[index].v_indices[0] - 1];
                                temp = transformed_vector3(ent->transform, temp.v);
                                temp = quaternion_mul_matrix(render->camera.matworld, temp);
                                a = temp.v;
                                temp.v = ent->obj->vertices[ent->obj->faces[index].v_indices[1] - 1];
                                temp = transformed_vector3(ent->transform, temp.v);
                                temp = quaternion_mul_matrix(render->camera.matworld, temp);
                                b = temp.v;
                                temp.v = ent->obj->vertices[ent->obj->faces[index].v_indices[2] - 1];
                                temp = transformed_vector3(ent->transform, temp.v);
                                temp = quaternion_mul_matrix(render->camera.matworld, temp);
                                c = temp.v;
                                if (id == ent->id && index == t_index)
                                {
                                    index++;
                                    continue;
                                }
                                t_texture   temp_t;
                                if (id == 5 && x == max.x - 5 && y == min.y + 5 && ent->id == 6)
                                {
                                    print_vector3(ray.dir);
                                    print_vector3(ray.origin);
                                    print_vector3(a);
                                    print_vector3(b);
                                    print_vector3(c);
                                }
                                if (id == 5 && x == max.x - 5 && y == min.y + 5 && ent->id == 6)
                                {
                                    print_vector3(p[0].v);
                                    print_vector3(p[1].v);
                                    print_vector3(p[2].v);
                                    printf("ent id %d index %d t index %d count %d\n", ent->id, index, t_index, ent->obj->face_count);
                                }
                                if (intersect_triangle(ray, &temp_t, a, b, c) == true)
                                {
                                    ol = true;
                                    if (id == 5 && x == max.x - 5 && y == min.y + 5 && ent->id == 6)
                                    {
                                        printf("HIT\n\n");
                                        print_vector3(p[0].v);
                                        print_vector3(p[1].v);
                                        print_vector3(p[2].v);
                                        printf("ent id %d index %d t index %d count %d\n", ent->id, index, t_index, ent->obj->face_count);
                                    }
                                    break;
                                }
                                index++;
                            }
                            if (ol == true)
                                break;
                        }
                        found++;
                    }
                    i++;
                }
                if (ol == false)
                {
                    render->lightmap->data[x + render->lightmap->size.x * y] = 255;
                }
                else
                    render->lightmap->data[x + render->lightmap->size.x * y] = 50;
                if (id == 5 && x == max.x - 5 && y == min.y + 5)
                {
                    render->lightmap->data[x + render->lightmap->size.x * y] = 74;
                }
			}
			x++;
		}
		y++;
	}
}

static void clip_draw_lightmap(t_sdlcontext *sdl, t_render *render, int id, int t_index)
{
	int			i;

	i = 0;
	while (i < render->occ_calc_tri_count)
	{
		ray_uv(render->occ_calc_tris[i], render, id, i);
		i++;
	}
}

/*
light get all entities around it.
malloc space to those entities triangles
*/

void calculate_pointlight(t_pointlight *pointlight, t_world *world, t_render *render)
{
    int i = 0;
    int found = 0;
    int index = 0;
	int j;
	t_entity *ent;
    t_triangle_polygon	**triangles;
    t_entity			**entities;
    triangles = malloc(sizeof(t_triangle_polygon *) * world->entitycache.existing_entitycount);
    entities = malloc(sizeof(t_entity *) * world->entitycache.existing_entitycount);
    while (found < world->entitycache.existing_entitycount)
    {
        ent = &world->entitycache.entities[i];
        if (ent->status != es_free)
        {
            if (ent->status == es_active)
            {
                triangles[index] = malloc(sizeof(t_triangle_polygon) * ent->obj->face_count);
                entities[index] = ent;
                j = 0;
                while (j < ent->obj->face_count)
                {
                    for (int e = 0; e < 3; e++)
                    {
                        t_quaternion temp;
                        temp.v = ent->obj->vertices[ent->obj->faces[j].v_indices[e] - 1];
                        temp = transformed_vector3(ent->transform, temp.v);
						temp = quaternion_mul_matrix(render->camera.matworld, temp);
                        triangles[index][j].p[e] = temp.v;
						triangles[index][j].uv[e] = ent->obj->uvs[ent->obj->faces[j].uv_indices[e] - 1];
                    }
                    j++;
                }
                index++;
            }
            found++;
        }
        i++;
    }
	bool		*map;
	t_lightmap *lightmap;
	i = 0;
	while (i < index)
	{
		ent = entities[i];
		j = 0;
		while (j < ent->obj->face_count)
		{
			t_vector2	max;
			t_point		tri[3];
			
			if (j == 0 || ent->obj->faces[j].materialindex != ent->obj->faces[j - 1].materialindex)
			{
				lightmap = &ent->lightmap[ent->obj->faces[j].materialindex];
				map = malloc(sizeof(bool) * lightmap->size.x * lightmap->size.y);
				bzero(map, sizeof(bool) * lightmap->size.x * lightmap->size.y);
			}
			max.x = -10000.0f;
			max.y = -10000.0f;
			for (int e = 0; e < 3; e++)
			{
				t_vector2 uv;
				uv.x = triangles[i][j].uv[e].x * (float)(ent->obj->materials[ent->obj->faces[j].materialindex].img->size.x - 1);
				uv.y = triangles[i][j].uv[e].y * (float)(ent->obj->materials[ent->obj->faces[j].materialindex].img->size.y - 1);
				if (uv.x > max.x)
					max.x = uv.x;
				if (uv.y > max.y)
					max.y = uv.y;
				tri[e].x = uv.x;
				tri[e].y = uv.y;
				triangles[i][j].uv[e].x = uv.x / (float)lightmap->size.x;
				triangles[i][j].uv[e].y = uv.y / (float)lightmap->size.y;
			}
			for (int n = 0; n < max.y; n++)
			{
				for (int m = 0; m < max.x; m++)
				{
					if (PointInTriangle((t_point){m, n}, tri[0], tri[1], tri[2]))
					{
						t_ray		ray;
						t_texture	temp_t;

						ray.origin = texcoord_to_loc(triangles[i][j].p, triangles[i][j].uv, (t_vector2){m/(float)lightmap->size.x, n/(float)lightmap->size.y});
               			ray.dir = vector3_sub(pointlight->origin, ray.origin);
						bool ol = false;
						for (int o = 0; o < index; o++)
						{
							for (int p = 0; p < entities[o]->obj->face_count; p++)
							{
								if (o == i && p == j)
								{
									continue; // if this isnt using for loop anymore remember to increment before continue
								}
								if (intersect_triangle(ray, &temp_t, triangles[o][p].p[0], triangles[o][p].p[1], triangles[o][p].p[2]) == true)
								{
									if (temp_t.w < 1.0f && temp_t.w > 0.01f)
									{
										ol = true;
										break;
									}
								}
							}
							if (ol == true)
								break;
						}
						if (ol == false && map[m + lightmap->size.x * n] == false)
						{
							float dist = vector3_dist(ray.origin, pointlight->origin);
							if (dist <= pointlight->radius)
							{
								dist = 1.0f - (dist / pointlight->radius);
								map[m + lightmap->size.x * n] = true;
								lightmap->data[m + lightmap->size.x * n] = ft_clamp((dist * 255) + lightmap->data[m + lightmap->size.x * n], 0, 255);
							}
						}
					}
				}
			}
			if (j + 1 == ent->obj->face_count || ent->obj->faces[j].materialindex != ent->obj->faces[j + 1].materialindex)
				free(map);
			j++;
		}
		i++;
	}
	i = 0;
	while (i < index)
	{
		free(triangles[i]);
		i++;
	}
	free(triangles);
	free(entities);
}

void update_pointlight_for_entity(t_sdlcontext sdl, t_render *render, t_entity *entity)
{
    int				index;
	t_object		*obj;
	t_quaternion	temp;
	t_vector2		max;


	obj = entity->obj;
    render->occ_calc_tri_count = 0;
	render_worldspace(render, entity);
	index = 0;
    printf("id %d\n", entity->id);
	while (index < obj->face_count)
	{
		t_triangle	tritransformed;

		tritransformed = (t_triangle){render->q[obj->faces[index].v_indices[0] - 1], render->q[obj->faces[index].v_indices[1] - 1], render->q[obj->faces[index].v_indices[2] - 1]};
		if (obj->uv_count != 0 && !render->wireframe)
		{
			tritransformed.t[0] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[0] - 1]);
			tritransformed.t[1] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[1] - 1]);
			tritransformed.t[2] = vector2_to_texture(obj->uvs[obj->faces[index].uv_indices[2] - 1]);
		}
        render->occ_calc_tris[render->occ_calc_tri_count++] = tritransformed;
		if (index + 1 == obj->face_count || obj->faces[index].materialindex != obj->faces[index + 1].materialindex)
		{
       //     if (index + 1 != obj->face_count && obj->faces[index].materialindex != obj->faces[index + 1].materialindex)
     //           exit(0);
			t_lightmap	*lightmap;
			lightmap = &entity->lightmap[obj->faces[index].materialindex];
			render->lightmap = lightmap;
			render->img = obj->materials[obj->faces[index].materialindex].img;
			clip_draw_lightmap(&sdl, render, entity->id, index);
			render->img = NULL;
			render->lightmap = NULL;
			render->map.img.data = NULL;
			render->occ_calc_tri_count = 0;
		}
		index++;
	}
}

               /* float w1 = ((lt[0].x * (lt[2].y - lt[0].y) + (y - lt[0].y) * (lt[2].x - lt[0].x) - x * (lt[2].y - lt[0].y)) / (float)((lt[1].y - lt[0].y) * (lt[2].x - lt[0].x) - (lt[1].x - lt[0].x) * (lt[2].y - lt[0].y)));
				float w2 = ((y - lt[0].y - w1 * (lt[1].y - lt[0].y))/(float)(lt[2].y - lt[0].y));
				if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
				{
					
					printf("first\n");
					ft_swap(&lt[1], &lt[2], sizeof(t_point));
                    ft_swap(&tes[1], &tes[2], sizeof(t_vector2));
					ft_swap(&p[1], &p[2], sizeof(t_point));
					ft_swap(&t[1], &t[2], sizeof(t_texture));

					w1 = ((lt[0].x * (lt[2].y - lt[0].y) + (y - lt[0].y) * (lt[2].x - lt[0].x) - x * (lt[2].y - lt[0].y)) / (float)((lt[1].y - lt[0].y) * (lt[2].x - lt[0].x) - (lt[1].x - lt[0].x) * (lt[2].y - lt[0].y)));
					w2 = ((y - lt[0].y - w1 * (lt[1].y - lt[0].y)) / (float)(lt[2].y - lt[0].y));
					if (isinf(w1) || isnan(w1) || isinf(w2) || isnan(w2))
						exit(0);
				}
				//printf("first w1 %f w2 %f\n", w1, w2);
				t_vector3 a = vector3_sub(p[1].v, p[0].v);
				t_vector3 b = vector3_sub(p[2].v, p[0].v);
				a = vector3_mul(a, w1);
				b = vector3_mul(b, w2);
				a = vector3_add(a, b);
				a = vector3_add(a, p[0].v);*/
