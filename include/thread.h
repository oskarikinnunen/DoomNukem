#ifndef THREAD_H
# define THREAD_H

# include <pthread.h>

#define THREAD 6

typedef struct s_thread
{
	void (*init)(void *);
	void *(*func)(void *);
	pthread_t	threads[THREAD];
	void		*structs;
	size_t		struct_size;
	size_t		count;
}	t_thread;

typedef struct s_test
{
	struct s_world *world;
	struct s_entity *entity;
} t_test;

void	*calculate_light_for_entities(t_test *ptr);
void	*calculate_texture_for_entity(struct s_entity *entity);

void	thread_set(t_thread *thread);
void	*thread_func_lighting(void *ptr);
void	thread_init_lighting(void *ptr);

#endif