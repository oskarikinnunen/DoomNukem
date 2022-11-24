#ifndef DEBUG_H
# define DEBUG_H

/*DEBUG FILES*/
void printf_tri(t_triangle tri);
void printf_quat(t_quaternion v);
void printf_vec(t_vector3 v);
void printf_matrix(t_mat4x4 m);
void printf_point(t_point p);
void printf_texture(t_texture t);
void printf_face(void *face);

#endif