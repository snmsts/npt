#include "thread.h"
#include "execute.h"

/*
 *  posix semaphore
 */
_g int trylock_semposix(semposix *sem)
{
	int result;

	result = sem_trywait(sem);
	if (result) {
		if (errno == EAGAIN) return 1;
		Debug("sem_trywait error");
		threaderror();
	}
	return 0;
}

_g int get_semposix(semposix *sem)
{
	int result;

	if (sem_getvalue(sem, &result)) {
		Debug("sem_post error");
		threaderror();
	}

	return result;
}


/*
 *  thread
 */
static void *start_routine(void *pvoid)
{
	Execute arg;

	arg = (Execute)pvoid;
	set_execute_local(arg);
	(*arg->routine)(arg);
	setstate_execute(arg, ThreadState_Finish);

	return NULL;
}

_g int create_thread(execfunction proc, Execute arg)
{
	if (pthread_create(&arg->handle, NULL, start_routine, (void *)arg)) {
		fprintf(stderr, "pthread_create error\n");
		return 1;
	}

	return 0;
}

_g int join_thread(threadhandle *handle)
{
	return pthread_join(*handle, NULL);
}

