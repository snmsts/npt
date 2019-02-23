#ifndef __DEGRADE_HEADER__
#define __DEGRADE_HEADER__

#ifdef LISP_DEGRADE
extern int DegradeArgc;
extern char **DegradeArgv;
extern char **DegradeEnv;
int degrade_printf(const char *fmt, ...);
int degrade_test(int check, const char *name);
void degrade_title(const char *name);
int degrade_testcheck(int check);
int degrade_testnormal(int (*call)(void));
void degrade_increment(void);

#define DegradeCheck(x) { if (x()) { degrade_increment(); return; } }
#define TestCheck(x)  { if (degrade_testcheck((x) != 0)) return 1; }
#define TestBreak(x)  TestCheck(x())
#define TestNormal(x) TestCheck(degrade_testnormal(x))
#define TITLE         degrade_title(__FILE__)
#define RETURN        { return 0; error: return 1; }


/*
 *  test
 */
#define test(x, y) { \
	if (degrade_test((x) != 0, (y))) { \
		goto error; \
	} \
}

#define testnot(x, y) { \
	test(!(x), (y)); \
}

#define testnormal(exec, name) { \
	codejump __jump; \
	Execute __ptr = Execute_Thread; \
	begin_switch(__ptr, &__jump); \
	if (codejump_run_p(&__jump)) { \
		exec; \
	} \
	end_switch(&__jump); \
	test(! codejump_error_p(&__jump), (name)); \
}

#define testabort(exec, name) { \
	codejump __jump; \
	Execute __ptr = Execute_Thread; \
	begin_switch(__ptr, &__jump); \
	if (codejump_run_p(&__jump)) { \
		exec; \
	} \
	end_switch(&__jump); \
	test(codejump_error_p(&__jump), (name)); \
}

#define testerror testabort

#endif
#endif
