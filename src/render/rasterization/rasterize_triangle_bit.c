#include "doomnukem.h"

bool fill_point_bit_tri_bot(t_sdlcontext *sdl, t_point *p)
{
	float			step[2];
	int				x;
	int				y;
	float			delta;
	bool			is_visible;

	is_visible = false;
	delta = 1.0f / ((float)(p[1].y - p[0].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	y = p[1].y;
	while (y >= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(p[1].y - y));
		int ax =  p[2].x + (step[1] * (float)(p[1].y - y));
		while(x <= ax)
		{
			if (!((sdl->bitmask.bitmask[(y / 4) * sdl->bitmask.chunk_size.x + (x / 8)] >> (((y % 4) * 8) + (x % 8))) & 1))
				return(true);
				//is_visible = true;
			//sdl->bitmask.bitmask[(y / 4) * (sdl->window_w / 8) + (x / 8)] |= 1UL << (((y % 4) * 8) + (x % 8));
			x++;
		}
		y--;
	}
	return(false);
}

bool fill_point_bit_tri_top(t_sdlcontext *sdl, t_point *p)
{
	float			step[2];
	int				x;
	int				y;
	float			delta;
	bool			is_visible;

	is_visible = false;
	delta = 1.0f/((float)(p[0].y - p[1].y));
	step[0] = (p[0].x - p[1].x) * delta;
	step[1] = (p[0].x - p[2].x) * delta;
	y = p[1].y;
	while (y <= p[0].y)
	{
		x = p[1].x + (step[0] * (float)(y - p[1].y));
		int ax =  p[2].x + (step[1] * (float)(y - p[1].y));
		while(x <= ax)
		{
			if (!((sdl->bitmask.bitmask[(y / 4) * sdl->bitmask.chunk_size.x + (x / 8)] >> (((y % 4) * 8) + (x % 8))) & 1))
				return(true);
			//	is_visible = true;
			//sdl->bitmask.bitmask[(y / 4) * (sdl->window_w / 8) + (x / 8)] |= 1UL << (((y % 4) * 8) + (x % 8));
			x++;
		}
		y++;
	}
	return(false);
}

bool	render_triangle_bit(t_sdlcontext *sdl, t_bit_triangle triangle)
{
	t_point			*p;
	t_point			p_split;
	t_point			p_temp;
	float			lerp;
	bool			is_visible = false;

	p = triangle.p;
	sort_point_tri(p);
	lerp = ((float)p[1].y - (float)p[2].y) / ((float)p[0].y - (float)p[2].y);
	p_split.x = p[2].x + (lerp * ((float)p[0].x - (float)p[2].x));
	p_split.y = p[1].y;
	if (p_split.x < p[1].x)
		ft_swap(&p[1], &p_split, sizeof(t_point));
	p_temp = p[2];
	p[2] = p_split;
	if (p[0].y != p[1].y)
		is_visible = fill_point_bit_tri_top(sdl, p);
	p[0] = p_temp;
	if (p[0].y != p[1].y && is_visible == fill_point_bit_tri_bot(sdl, p))
		return(is_visible);
	return(false);
}
