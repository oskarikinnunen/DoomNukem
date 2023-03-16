#include "doomnukem.h"

//TODO: RENAME
void	thread_set(t_thread *thread)
{
	int			i;

	i = 0;
	while (i < THREAD)
	{
		thread->init(&(thread->structs[i * thread->struct_size]));
		pthread_create(&thread->threads[i], NULL, thread->func, &(thread->structs[i * thread->struct_size]));
		i++;
	}
	i = 0;
	while (i < THREAD)
	{
		pthread_join(thread->threads[i], NULL);
		i++;
	}
}
