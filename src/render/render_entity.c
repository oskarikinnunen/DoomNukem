/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_entity.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 11:05:07 by vlaine            #+#    #+#             */
/*   Updated: 2023/01/11 13:29:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomnukem.h"
 
#include "bresenham.h"
#include "objects.h"
#include "vectors.h"

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
}


static t_vector2 proj_quaternion_to_screenspace(t_sdlcontext *sdl, t_quaternion proj_q)
{
	proj_q.v = vector3_div(proj_q.v, proj_q.w);
	proj_q.v = vector3_negative(proj_q.v);
	t_vector3 voffsetview = (t_vector3){1.0f, 1.0f, 0.0f};
	proj_q.v = vector3_add(proj_q.v, voffsetview);
	proj_q.v.x *= 0.5f * (float)sdl->window_w;
	proj_q.v.y *= 0.5f * (float)sdl->window_h;
	return ((t_vector2){proj_q.v.x, proj_q.v.y});
}

typedef struct quatline
{
	t_quaternion	start;
	t_quaternion	end;
}	t_quatline;

t_vector3 pplane()
{
	return ((t_vector3){.z = 0.1f});
}

t_vector3 nplane()
{
	return ((t_vector3){.z = 0.1f});
}

bool	clip_line_against_lineplane(t_line *line, t_line plane)
{
	float	fdist1;
	float	fdist2;
	float	lerp;
	t_line temp;


	temp.start = plane.end;
	temp.end = plane.start;

	//plane = temp;

	fdist1 = vector2_fdist_to_plane(line->start, plane.start, plane.end);
	fdist2 = vector2_fdist_to_plane(line->end, plane.start, plane.end);
	if (fdist1 < 0.0f && fdist2 < 0.0f)
		return (false);
	if (fdist1 < 0.0f || fdist2 < 0.0f)
	{
		if (fdist1 < 0.0f)
		{
			lerp = vector2_line_intersect_plane(plane.start, plane.end, line->start, line->end);
			line->start = vector2_lerp(line->start, line->end, lerp);
		}
		else
		{
			lerp = vector2_line_intersect_plane(plane.start, plane.end, line->end, line->start);
			line->end = vector2_lerp(line->end, line->start, lerp);
		}
	}
	return (true);
}

bool	clip_quatline_to_zplane(t_quatline *ql)
{
	t_vector3		pp;
	t_vector3		pn;
	float			lerp;
	float			fdist1;
	float			fdist2;

	pp = (t_vector3){.z = 0.1f};
	pn = (t_vector3){.z = 1.0f};
	fdist1 = vector3_fdist_to_plane(ql->start.v, pn, pp);
	fdist2 = vector3_fdist_to_plane(ql->end.v, pn, pp);
	if (fdist1 < 0.0f && fdist2 < 0.0f)
		return (false);
	if (fdist1 < 0.0f || fdist2 < 0.0f)
	{
		if (fdist1 < 0.0f)
		{
			lerp = line_intersect_plane(pp, pn, ql->start.v, ql->end.v);
			ql->start = lerp_quaternion(ql->start, ql->end, lerp);
		}
		else
		{
			lerp = line_intersect_plane(pp, pn, ql->end.v, ql->start.v);
			ql->end = lerp_quaternion(ql->end, ql->start, lerp);
		}
	}
	return (true);
}

static t_line newline(t_vector2 start, t_vector2 end)
{
	t_line	l;
	l.start = start;
	l.end = end;
	return (l);
}

void render_ray(t_sdlcontext *sdl, t_vector3 from, t_vector3 to)
{
	t_quaternion	q1;
	t_quaternion	q2;
	t_line			l;

	q1 = vector3_to_quaternion(from);
	q2 = vector3_to_quaternion(to);
	q1 = quaternion_mul_matrix(sdl->render.camera.matworld, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.matworld, q2);
	q1 = quaternion_mul_matrix(sdl->render.camera.matview, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.matview, q2);
	q1 = quaternion_mul_matrix(sdl->render.camera.matproj, q1);
	q2 = quaternion_mul_matrix(sdl->render.camera.matproj, q2);
	t_quatline ql;
	ql.start = q1;
	ql.end = q2;
	//if (vector3_fdist_to_plane(ql.start.v, nplane(), pplane()) < 0.0f
	//	&& vector3_fdist_to_plane(ql.end.v, nplane(), pplane()) < 0.0f)
	//	return ;
	if (!clip_quatline_to_zplane(&ql))
		return ;
	l.start = proj_quaternion_to_screenspace(sdl, ql.start);
	l.end = proj_quaternion_to_screenspace(sdl, ql.end);
	t_line plane1, plane2, plane3, plane4;
	/*
	//TODO: this doesn't currently clip against screen borders
	plane1 = newline((t_vector2){0.0f, 0.0f}, (t_vector2){0.0f, 1.0f});
	plane2 = newline((t_vector2){0.0f, (float)(sdl->window_h) - 1.0f}, (t_vector2){0.0f, -1.0f});
	plane3 = newline((t_vector2){0.0f, 0.0f}, (t_vector2){1.0f, 0.0f});
	plane4 = newline((t_vector2){(float)(sdl->window_w) - 1.0f, 0.0f}, (t_vector2){-1.0f, 0.0f});
	if (!clip_line_against_lineplane(&l, plane1))
		return ;
	if (!clip_line_against_lineplane(&l, plane2))
		return ;
	if (!clip_line_against_lineplane(&l, plane3))
		return ;
	if (!clip_line_against_lineplane(&l, plane4))
		return ;
	if (l.start.x < 0.0f || l.end.x > sdl->window_w)
	{
		printf("line start: %f %f end: %f %f \n", l.start.x, l.start.y, l.end.x, l.end.y);
	}*/
	drawline(*sdl, vector2_to_point(l.start), vector2_to_point(l.end), sdl->render.gizmocolor);
}

//DEPRECATED, USE render_gizmo3d/render_gizmo2d INSTEAD
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
		vector3_to_screenspace(v2tov3(pos), *sdl), size, color);
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