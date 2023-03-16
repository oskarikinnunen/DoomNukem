#include "doomnukem.h"

void *thread_func_lighting(void *ptr)
{
	calculate_light_for_entities(ptr);
	return(NULL);
}

void thread_init_lighting(void *ptr)
{
	return;
}
