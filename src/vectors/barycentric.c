#include "vectors.h"

t_barycentric	get_barycentric(t_vector2 *p)
{
    t_barycentric   b;

    b.b1 = p[2].y - p[0].y;
    b.b2 = p[2].x - p[0].x;
    b.b3 = p[1].y - p[0].y;
    b.b1x = p[0].x * b.b1;
    b.div = (b.b3 * b.b2 - (p[1].x - p[0].x) * b.b1);
}

t_vector2	fast_barycentric(t_vector2 *p, t_barycentric b, t_vector2 v)
{
    float       b4;
    t_vector2   w;

    b4 = v.y - p[0].y;
    w.x = (b.b1x + b4 * b.b2 - v.x * b.b1) / b.div;
    w.y = (b4 - w.x * b.b3) / b.b1;
    return(w);
}

t_vector2	barycentric_coordinates(t_vector2 *p, t_vector2 v)
{
    float       b1;
    float       b2;
    float       b3;
    float       b4;
    t_vector2   w;

    b1 = p[2].y - p[0].y;
    b2 = p[2].x - p[0].x;
    b3 = p[1].y - p[0].y;
    b4 = v.y - p[0].y;

    w.x = (p[0].x * b1 + b4 * b2 - v.x * b1) / (b3 * b2 - (p[1].x - p[0].x) * b1);
    w.y = (b4 - w.x * b3) / b1;
    return(w);
}
