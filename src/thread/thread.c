#include "doomnukem.h"

//TODO: RENAME
void	multithread_start(t_thread *thread)
{
	int			i;
	void		*ptr;

	i = 0;
	while (i < thread->count)
	{
		ptr = &(thread->structs[i * thread->struct_size]);
		pthread_create(&thread->threads[i], NULL, thread->func, ptr);
		i++;
	}
	i = 0;
	while (i < thread->count)
	{
		pthread_join(thread->threads[i], NULL);
		i++;
	}
	ft_bzero(thread->structs, thread->struct_size * THREAD);
	thread->count = 0;
}
