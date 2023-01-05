/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_entity.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/05 15:43:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
#include "bresenham.h"
#include "objects.h"
#include "vectors.h"

//TODO: Legacy occlusion using deprecated after occlusion gets updated
void clipped(t_render *render, t_sdlcontext sdl)
{
	int i = 0;
	int start = 0;
	int end = 0;

	t_triangle	triangles[200];
	t_triangle	clipped[2];
	while (i < render->occ_calc_tri_count)
	{
		triangles[end++] = render->occ_calc_tris[i];
		int nnewtriangles = 1;
		for (int p = 0; p < 4; p++)
		{
			int ntristoadd = 0;
			while (nnewtriangles > 0)
			{
				t_triangle test;
				test = triangles[start++];
				nnewtriangles--;
				switch (p)
				{
				case 0: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){0.0f, 1.0f, 0.0f}, test, clipped); break;
				case 1: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, (float)(sdl.window_h * sdl.resolution_scaling) - 1.0f, 0.0f}, (t_vector3){0.0f, -1.0f, 0.0f}, test, clipped); break;
				case 2: ntristoadd = clip_triangle_against_plane((t_vector3){0.0f, 0.0f, 0.0f}, (t_vector3){1.0f, 0.0f, 0.0f}, test, clipped); break;
				case 3: ntristoadd = clip_triangle_against_plane((t_vector3){(float)(sdl.window_w * sdl.resolution_scaling) *  - 1.0f, 0.0f, 0.0f}, (t_vector3){-1.0f, 0.0f, 0.0f}, test, clipped); break;
				}
				for (int w = 0; w < ntristoadd; w++)
				{
					triangles[end++] = clipped[w];
				}
			}
			nnewtriangles = end - start;
		}
		while (start < end)
		{
			render->occ_draw_tris[render->occ_tri_count++] = triangles[start++];
		}
		start = 0;
		end = 0;
		i++;
	}
}

static t_quaternion quaternion_to_screenspace(t_mat4x4 matproj, t_quaternion q, t_sdlcontext sdl)
{
	t_quaternion	proj_q;

	proj_q = quaternion_mul_matrix(matproj, q);


	proj_q.v = vector3_div(proj_q.v, proj_q.w);

	proj_q.v = vector3_negative(proj_q.v);

	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);

	proj_q.v.x *= 0.5f * (float)sdl.window_w;
	proj_q.v.y *= 0.5f * (float)sdl.window_h;

	return(proj_q);
}

uint32_t shade(uint32_t clr, float norm)
{
	float		mul;
	uint32_t	final;

	//return (clr);
	mul = ft_clampf(norm, 0.75f, 1.0f);
	final = (clr & 0xFF) * mul;
	final += (uint32_t)((clr >> 8 & 0xFF) * mul) << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * mul) << 16;
	return (final);
}

void render_entity(t_sdlcontext *sdl, t_render *render, t_entity *entity)
{
	render_worldspace(render, entity);
	render_quaternions(sdl, render, entity);
	render->rs.render_count++;
}

void render_ray(t_sdlcontext *sdl, t_vector3 from, t_vector3 to)
{
	drawline(*sdl, vector3_to_screenspace(from, *sdl), vector3_to_screenspace(to, *sdl), sdl->render.gizmocolor);
}

void render_gizmo(t_sdlcontext sdl, t_render render, t_vector3 pos, int size)
{
	drawcircle(sdl, vector3_to_screenspace(pos, sdl), size, render.gizmocolor);
}

void render_gizmo3d(t_sdlcontext *sdl, t_vector3 pos, int size, uint32_t color)
{
	drawcircle(*sdl, vector3_to_screenspace(pos, *sdl), size, color);
}

void render_gizmo2d(t_sdlcontext *sdl, t_vector2 pos, int size, uint32_t color)
{
	drawcircle(*sdl,
		vector3_to_screenspace(vector2_to_vector3(pos), *sdl), size, color);
}

static t_vector3 lookdirection2(t_vector2 angle)
{
	t_quaternion	temp;
	t_mat4x4		matcamerarot;

	matcamerarot = matrix_makerotationz(angle.x);
	temp = quaternion_mul_matrix(matcamerarot, (t_quaternion){1.0f, 0.0f, 0.0f, 1.0f});

	return (temp.v);
}

//TODO: this don't work
void	draw_screen_to_worldspace_ray(t_sdlcontext sdl, t_render render, t_point origin, t_vector2 og_angle)
{
	t_camera	c;
	t_vector3	curstep;
	t_vector3	dir;
	t_vector2	angle;
	t_entity	ent;
	int	debug_iter;
	
	c = render.camera;
	curstep = c.position;
	angle.y = (((sdl.window_h / 2) - (float)origin.y) / (sdl.window_h / 2.0f)); //[1][1] should be FOV in radians;
	angle.y = angle.y * ft_degtorad(45.0f);

	angle.x = (((sdl.window_w / 2) - (float)origin.x) / (sdl.window_w / 2.0f));
	angle.x = angle.x * (ft_degtorad(45.0f) * ((float)sdl.window_w / (float)sdl.window_h));
	//angle.x = 0.0f;
	//angle.x = 0;
	dir = vector3_zero();
	dir.y = -lookdirection((t_vector2){og_angle.x, og_angle.y + angle.y}).y;
	dir.x = -lookdirection2((t_vector2){og_angle.x + angle.x, og_angle.y + angle.y}).x;
	dir.y *= c.position.z * 1.4f;
	dir.x *= c.position.z * ((float)sdl.window_w / (float)sdl.window_h);
	dir.z = c.position.z;
	/*dir.x *= 2.0f;
	dir.y *= 2.0f;
	dir.z = 2.0f;*/ //WORKS
	debug_iter = 0;
	while (curstep.z > 0.0f && debug_iter < 400)
	{
		curstep = vector3_movetowards(curstep, dir, 0.01f);
		ent.transform.position = curstep;
		ent.transform.scale = vector3_one();
		//render_gizmo(sdl, render, &ent);
		debug_iter++;
	}
}