#ifndef DEBUG_H
# define DEBUG_H

/*DEBUG FILES*/
void print_matrix(t_mat4x4 m);
void print_point(t_point p);
void print_vector3(t_vector3 v);
void print_vector2(t_vector2 v);
void print_quat(t_quaternion q);
void print_tri(t_triangle tri);
void print_texture(t_texture t);
void print_face(void *face);
void print_render_statistics(t_render_statistics rs);


#endif