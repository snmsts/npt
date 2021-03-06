#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "build.h"
#include "define.h"
#include "execute.h"
#include "local.h"
#include "thread.h"

#ifdef LISP_DEBUG
#define EXECUTE_SIZE      2
#define EXECUTE_PLUS      3
#define EXECUTE_BODY      4
#else
#define EXECUTE_SIZE      0x010000
#define EXECUTE_PLUS      0x010000
#define EXECUTE_BODY      0x010000
#endif

#define SizeLimit (1024UL * 1024UL)

/*
 *  execute
 */
static struct execute **ExecuteArray = NULL;
static mutexlite ExecuteMutex;
static condlite ExecuteCond;
static size_t ExecuteSize;
static size_t ExecutePosition;

#ifdef LISP_DEGRADE
struct execute ***Degrade_execute_Execute(void) { return &ExecuteArray; }
size_t *Degrade_execute_Size(void) { return &ExecuteSize; }
size_t *Degrade_execute_Position(void) { return &ExecutePosition; }
#endif


/*
 *  execute
 */
static void init_execute_local(void)
{
	make_threadlocal(&ThreadLocal_Execute);
	make_threadlocal(&ThreadLocal_Index);
	make_threadlocal(&ThreadLocal_Local);
}
static void free_execute_local(void)
{
	destroy_threadlocal(ThreadLocal_Execute);
	destroy_threadlocal(ThreadLocal_Index);
	destroy_threadlocal(ThreadLocal_Local);
}
_g void set_execute_local(struct execute *ptr)
{
	set_threadlocal(ThreadLocal_Execute, (void *)ptr);
	set_threadlocal(ThreadLocal_Index, (void *)&ptr->index);
	set_threadlocal(ThreadLocal_Local, (void *)ptr->local);
}

static struct execute *allocmembit(size_t index)
{
	struct execute *bit;

	bit = malloctype(struct execute);
	if (bit == NULL) {
		Debug("malloctype error");
		return NULL;
	}
	if (make_mutexlite(&bit->mutex)) {
		Debug("make_mutexlite error");
		free(bit);
		return NULL;
	}
	clearpoint(bit);
	bit->state = ThreadState_Empty;
	bit->index = index;
	bit->property = 0;
	bit->signal = ExecuteControl_Run;
	bit->taginfo = NULL;
	bit->result = 0;

	return bit;
}

static void freemembit(struct execute *bit)
{
	destroy_mutexlite(&bit->mutex);
	free(bit);
}

static struct execute **allocbuffer(size_t size)
{
	size_t i, k;
	struct execute **ptr, *bit;

	ptr = mallocsize(struct execute *, size);
	if (ptr == NULL) {
		Debug("mallocsize error");
		return NULL;
	}
	for (i = 0; i < size; i++) {
		bit = allocmembit(i);
		if (bit == NULL) {
			Debug("allocmembit error");
			for (k = 0; k < i; k++)
				freemembit(ptr[k]);
			free(ptr);
			return NULL;
		}
		ptr[i] = bit;
	}

	return ptr;
}

static void freebuffer(struct execute **ptr)
{
	size_t i;

	if (ptr) {
		for (i = 0; i < ExecuteSize; i++)
			freemembit(ptr[i]);
		free(ptr);
	}
}

static int init_mainthread(struct execute **ptr, size_t size)
{
	struct execute *bit;
	jmp_buf *exec;
	LocalRoot local;

	exec = malloctype(jmp_buf);
	if (exec == NULL) {
		Debug("malloctype error");
		return 1;
	}

	local = make_local(size);
	if (local == NULL) {
		Debug("make_local error");
		free(exec);
		return 1;
	}

	bit = ptr[0];  /* main thread */
	/* thread info */
	bit->state = ThreadState_Run;
	bit->routine = 0;
	cleartype(bit->handle);
#ifdef LISP_THREAD_WINDOWS
	cleartype(bit->handleid);
#endif

	/* lisp info */
	bit->exec = exec;
	bit->local = local;
	bit->control = Unbound;

	/* threadlocal */
	set_execute_local(bit);

	return 0;
}

_g int init_execute(size_t size)
{
	struct execute **ptr;

	if (ExecuteArray) {
		Debug("ExecuteArray error.");
		return 1;
	}

	/* size check */
	if (size < SizeLimit)
		size = SizeLimit;

	/* execute structure */
	ptr = allocbuffer(EXECUTE_SIZE);
	if (ptr == NULL) {
		Debug("allocbuffer error");
		return 1;
	}

	/* sync object */
	if (make_mutexlite(&ExecuteMutex)) {
		Debug("make_mutexlite error");
		goto error1;
	}
	make_condlite(&ExecuteCond);

	/* threadlocal */
	init_execute_local();

	/* thread index 0 */
	if (init_mainthread(ptr, size)) {
		Debug("init_mainthread error");
		goto error3;
	}

	/* Global variables */
	ExecuteArray = ptr;
	ExecuteSize = EXECUTE_SIZE;
	ExecutePosition = 1;

	return 0;

error3:
	freebuffer(ptr);
	destroy_condlite(&ExecuteCond);
error1:
	destroy_mutexlite(&ExecuteMutex);
	return 1;
}

_g void free_execute(void)
{
	size_t i;
	struct execute *bit;

	if (ExecuteArray == NULL) return;

	/* ExecuteArray */
	for (i = 0; i < ExecuteSize; i++) {
		bit = ExecuteArray[i];
		if (bit->state != ThreadState_Empty) {
			free_local(bit->local);
			free(bit->exec);
		}
	}

	/* threadlocal */
	free_execute_local();

	/* Global variables */
	destroy_condlite(&ExecuteCond);
	destroy_mutexlite(&ExecuteMutex);
	freebuffer(ExecuteArray);
	ExecuteArray = 0;
	ExecuteSize = 0;
}

static int extendmemory(void)
{
	size_t size, i, k;
	struct execute **ptr, *bit;

	size = ExecuteSize + EXECUTE_PLUS;
	ptr = reallocsize(ExecuteArray, struct execute *, size);
	if (ptr == NULL) {
		Debug("reallocsize error");
		return 1;
	}
	for (i = ExecuteSize; i < size; i++) {
		bit = allocmembit(i);
		if (bit == NULL) {
			Debug("allocmembit error");
			for (k = ExecuteSize; k < i; k++)
				freemembit(ptr[k]);
			/* for realloc */
			ExecuteArray = ptr;
			/* try recovery */
			ptr = reallocsize(ExecuteArray, struct execute *, ExecuteSize);
			if (ptr) ExecuteArray = ptr;
			return 1;
		}
		ptr[i] = bit;
	}
	ExecuteArray = ptr;
	ExecutePosition = ExecuteSize;
	ExecuteSize = size;

	return 0;
}

static int findempty(struct execute **ret)
{
	size_t index;

	/* first try */
	for (index = ExecutePosition; index < ExecuteSize; index++) {
		if (ExecuteArray[index]->state == ThreadState_Empty) {
			ExecutePosition = index;
			*ret = ExecuteArray[index];
			return 1;
		}
	}

	/* second try */
	for (index = 1; index < ExecutePosition; index++) {
		if (ExecuteArray[index]->state == ThreadState_Empty) {
			ExecutePosition = index;
			*ret = ExecuteArray[index];
			return 1;
		}
	}

	return 0;
}

static int findstate(struct execute **ptr)
{
	lock_mutexlite(&ExecuteMutex);

	/* find state=ThreadState_Empty */
	if (findempty(ptr)) goto finish;

	/* extend memory */
	if (extendmemory()) {
		Debug("expandmemory error");
		unlock_mutexlite(&ExecuteMutex);
		return 1;
	}
	*ptr = ExecuteArray[ExecutePosition];

finish:
	(*ptr)->state = ThreadState_Run;
	unlock_mutexlite(&ExecuteMutex);

	return 0;
}

_g void setstate_execute(struct execute *ptr, enum ThreadState value)
{
	lock_mutexlite(&ExecuteMutex);
	ptr->state = value;
	unlock_mutexlite(&ExecuteMutex);
}

_g int make_execute(execfunction proc, struct execute **ret, size_t size)
{
	struct execute *ptr;

	/* size check */
	if (size < SizeLimit)
		size = SizeLimit;

	/* alloc */
	if (findstate(&ptr)) {
		Debug("findstate error");
		return 1;
	}

	/* thread */
	ptr->routine = proc;
	if (create_thread(proc, ptr)) {
		Debug("create_thread error");
		setstate_execute(ptr, ThreadState_Empty);
		return 1;
	}
	if (ret) *ret = ptr;

	return 0;
}

_g int join_execute(struct execute *ptr)
{
	int result;

	lock_mutexlite(&ExecuteMutex);
	switch (ptr->state) {
		case ThreadState_Empty:
			break;

		case ThreadState_Run:
			ptr->state = ThreadState_Join;
			goto join;

		case ThreadState_Finish:
			goto join;

		case ThreadState_Join:
			break;

		default:
			Debug("join_thread  state error");
			goto error;
	}
	unlock_mutexlite(&ExecuteMutex);
	return 0;

join:
	unlock_mutexlite(&ExecuteMutex);
	result = join_thread(&ptr->handle);
	if (result) {
		Debug("join_thread error");
		return 1;
	}
	setstate_execute(ptr, ThreadState_Empty);
	return 0;

error:
	unlock_mutexlite(&ExecuteMutex);
	return 1;
}

_g size_t count_execute(void)
{
	size_t i, count;

	count = 0;
	for (i = 0; i < ExecuteSize; i++) {
		if (ExecuteArray[i]->state != ThreadState_Empty)
			count++;
	}

	return count;
}

_g int joinindex_execute(size_t index)
{
	return join_execute(ExecuteArray[index]);
}

_g struct execute *getexecute(size_t index)
{
	struct execute *result;

	lock_mutexlite(&ExecuteMutex);
	if (ExecuteSize <= index) {
		unlock_mutexlite(&ExecuteMutex);
		Debug("index error");
		return NULL;
	}
	result = ExecuteArray[index];
	unlock_mutexlite(&ExecuteMutex);

	return result;
}

_g void exitexecute(struct execute *ptr, lispcode code)
{
	if (ptr == NULL || (! GetPropertyExecute(ptr, LISPPROP_JUMP))) {
		Debug("exitexecute error");
		exitindex(0, LISPCODE_ABORT);
	}
	exit_code(ptr, code);
}

_g void exitindex(size_t index, lispcode code)
{
	struct execute *ptr;

	ptr = getexecute(index);
	if (ptr == NULL) {
		Debug("getexecute error");
		abortindex(0);
		return;
	}
	exitexecute(ptr, code);
}

_g void abortexecute(struct execute *ptr)
{
	if (ptr == NULL || (! GetPropertyExecute(ptr, LISPPROP_JUMP))) {
		if (ptr == NULL || ptr->index == 0) {
			Debug("abort.");
			exit(1);
		}
		else {
			abortindex(0);
		}
	}
	SetPropertyExecute(ptr, LISPPROP_ABORT, 1);
	exitexecute(ptr, LISPCODE_ABORT);
}

_g void abortindex(size_t index)
{
	struct execute *ptr;

	ptr = getexecute(index);
	if (ptr == NULL) {
		Debug("getexecute error");
		if (index == 0) {
			Debug("abort.");
			exit(1);
		}
		else {
			abortindex(0);
		}
		return;
	}
	abortexecute(ptr);
}


/*
 *  codejump
 */
_g int begin_code_check(Execute ptr, lispcode *code)
{
	if (GetPropertyExecute(ptr, LISPPROP_JUMP)) {
		*code = LISPCODE_CONFLICT;
		return 0;
	}
	else {
		SetPropertyExecute(ptr, LISPPROP_JUMP, 1);
		return 1;
	}
}

_g void end_code(Execute ptr)
{
	SetPropertyExecute((ptr), LISPPROP_JUMP, 0);
	ClearJmpBuf(ptr->exec);
}
_g void end_code_thread(void)
{
	end_code(Execute_Thread);
}

_g int code_run_p(lispcode code)
{
	return code == LISPCODE_EXECUTE;
}
_g int code_end_p(lispcode code)
{
	return code == LISPCODE_SUCCESS;
}
_g int code_error_p(lispcode code)
{
	return code >= LISPCODE_ERROR;
}

_g void begin_switch_check(Execute ptr, codejump *code)
{
	Check(! GetPropertyExecute(ptr, LISPPROP_JUMP), "begin_switch error");
	code->ptr = ptr;
	CopyJmpBuf(&(code->jump), ptr->exec);
}

_g void end_switch(codejump *code)
{
	Execute ptr = code->ptr;
	CopyJmpBuf(ptr->exec, &(code->jump));
}

_g int codejump_run_p(codejump *code)
{
	return code_run_p(code->code);
}
_g int codejump_end_p(codejump *code)
{
	return code_end_p(code->code);
}
_g int codejump_error_p(codejump *code)
{
	return code_error_p(code->code);
}

_g int codejump_control_p(codejump *code)
{
	return code_error_p(code->code) && (code->code != LISPCODE_CONTROL);
}

_g void exit_code(Execute ptr, lispcode code)
{
	longjmp(*(ptr->exec), code);
}
_g void exit_code_thread(lispcode code)
{
	exit_code(Execute_Thread, code);
}

_g void break_code(Execute ptr)
{
	exit_code(ptr, LISPCODE_SUCCESS);
}
_g void break_code_thread(void)
{
	break_code(Execute_Thread);
}

_g void throw_code(Execute ptr, lispcode code)
{
	if (code_error_p(code))
		exit_code(ptr, code);
}
_g void throw_code_thread(lispcode code)
{
	throw_code(Execute_Thread, code);
}
_g void throw_switch(codejump *code)
{
	throw_code(code->ptr, code->code);
}
_g int equal_control_restart(Execute ptr, addr control)
{
	struct taginfo_struct *str = ptr->taginfo;
	return str && str->control == control;
}


/*
 *  gc sync
 */
_g void gcstate_execute(void)
{
	size_t i;
	struct execute *ptr;

	lock_mutexlite(&ExecuteMutex);
	for (i = 0; i < ExecuteSize; i++) {
		ptr = ExecuteArray[i];
		switch (ptr->state) {
			case ThreadState_Empty:
			case ThreadState_Finish:
			case ThreadState_Join:
				continue;

			case ThreadState_Run:
				ptr->state = ThreadState_Signal;
				break;

			default:
				Debug("state error");
				exitthis(LISPCODE_ERROR);
				break;
		}
	}
	broadcast_condlite(&ExecuteCond);
	unlock_mutexlite(&ExecuteMutex);
}

_g void gcstart_execute(struct execute *ptr)
{
	size_t i;

	lock_mutexlite(&ExecuteMutex);
	ptr->state = ThreadState_GcStart;
loop:
	for (i = 0; i < ExecuteSize; i++) {
		switch (ExecuteArray[i]->state) {
			case ThreadState_Signal:
				wait_condlite(&ExecuteCond, &ExecuteMutex);
				goto loop;

			case ThreadState_Run:
				Debug("state error");
				exitthis(LISPCODE_ERROR);
				break;

			default:
				break;
		}
	}
	unlock_mutexlite(&ExecuteMutex);
}

_g void gcwait_execute(struct execute *ptr)
{
	lock_mutexlite(&ExecuteMutex);
	while (ptr->state != ThreadState_Run)
		wait_condlite(&ExecuteCond, &ExecuteMutex);
	unlock_mutexlite(&ExecuteMutex);
}

_g void gcend_execute(void)
{
	size_t i;
	struct execute *ptr;

	lock_mutexlite(&ExecuteMutex);
	for (i = 0; i < ExecuteSize; i++) {
		ptr = ExecuteArray[i];
		if (ptr->state == ThreadState_GcStart)
			ptr->state = ThreadState_Run;
	}
	broadcast_condlite(&ExecuteCond);
	unlock_mutexlite(&ExecuteMutex);
}

_g void foreach_execute(void (*call)(struct execute *))
{
	size_t i;
	struct execute *ptr;

	for (i = 0; i < ExecutePosition; i++) {
		ptr = ExecuteArray[i];
		if (ptr->state != ThreadState_Empty)
			call(ptr);
	}
}

_g int foreach_check_execute(int (*call)(struct execute *))
{
	size_t i;
	struct execute *ptr;

	for (i = 0; i < ExecutePosition; i++) {
		ptr = ExecuteArray[i];
		if (ptr->state != ThreadState_Empty) {
			if (call(ptr))
				return 1;
		}
	}

	return 0;
}


/*
 *  exit
 */
_g void exit_execute(int value)
{
	Execute ptr;

	ptr = getexecute(0);
	ptr->result = value;
	exitexecute(ptr, LISPCODE_EXIT);
}

