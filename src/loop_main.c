#include "condition.h"
#include "cons.h"
#include "constant.h"
#include "loop_main.h"
#include "symbol.h"

_g void make_loop_main(struct loop_main *ptr, addr form, addr init, addr named)
{
	ptr->form = form;
	ptr->init = init;
	ptr->named = named;
}

static void loop_main_push_form(struct loop_main *ptr, addr pos)
{
	cons_heap(&(ptr->form), pos, ptr->form);
}

static void loop_main_push_init(struct loop_main *ptr, addr pos)
{
	cons_heap(&(ptr->init), pos, ptr->init);
}

static void loop_push_main_do(struct loop_main *ptr, addr list)
{
	addr pos;

	while (list != Nil) {
		GetCons(list, &pos, &list);
		loop_main_push_form(ptr, pos);
	}
}

static void loop_push_main_return(struct loop_main *ptr, addr list)
{
	addr expr, retfrom, x;

	list_bind(list, &expr, NULL);
	/* `(return-from ,named ,expr) */
	GetConst(COMMON_RETURN_FROM, &retfrom);
	list_heap(&x, retfrom, ptr->named, expr, NULL);
	loop_main_push_form(ptr, x);
}

static void loop_push_main_collect(struct loop_main *ptr, addr list)
{
	addr form, into, value, setq, push, append, lsym, x;
	addr floop, fsym, nreverse;

	list_bind(list, &form, &into, NULL);
	if (into == Unbound) {
		/* `(setq function-loop (function nreverse)) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(SYSTEM_FUNCTION_LOOP, &floop);
		GetConst(COMMON_FUNCTION, &fsym);
		GetConst(COMMON_NREVERSE, &nreverse);
		list_heap(&fsym, fsym, nreverse, NULL);
		list_heap(&x, setq, floop, fsym, NULL);
		loop_main_push_init(ptr, x);
		/* `(push ,form value-loop) */
		GetConst(COMMON_PUSH, &push);
		GetConst(SYSTEM_VALUE_LOOP, &value);
		list_heap(&x, push, form, value, NULL);
		loop_main_push_form(ptr, x);
	}
	else {
		/* `(setq ,into (append ,into (list ,form))) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(COMMON_APPEND, &append);
		GetConst(COMMON_LIST, &lsym);
		list_heap(&lsym, lsym, form, NULL);
		list_heap(&append, append, into, lsym, NULL);
		list_heap(&x, setq, into, append, NULL);
		loop_main_push_form(ptr, x);
	}
}

static void loop_let_main_collect(addr *form, addr list)
{
	addr value, into, let;

	list_bind(list, &value, &into, NULL);
	if (into != Unbound) {
		/* (let (,into) ,form) */
		GetConst(COMMON_LET, &let);
		list_heap(&into, into, NULL);
		list_heap(form, let, into, *form, NULL);
	}
}

static void loop_push_main_append(struct loop_main *ptr, addr list)
{
	addr form, into, value, setq, push, append, x;
	addr floop, lambda, apply, fsym, nreverse;

	list_bind(list, &form, &into, NULL);
	if (into == Unbound) {
		/* `(setq function-loop
		 *    (lambda (x)
		 *      (apply (function append) (nreverse x))))
		 */
		GetConst(COMMON_SETQ, &setq);
		GetConst(SYSTEM_FUNCTION_LOOP, &floop);
		GetConst(COMMON_LAMBDA, &lambda);
		GetConst(COMMON_APPLY, &apply);
		GetConst(COMMON_FUNCTION, &fsym);
		GetConst(COMMON_APPEND, &append);
		GetConst(COMMON_NREVERSE, &nreverse);
		make_symbolchar(&x, "X");
		list_heap(&nreverse, nreverse, x, NULL);
		list_heap(&fsym, fsym, append, NULL);
		list_heap(&apply, apply, fsym, nreverse, NULL);
		list_heap(&x, x, NULL);
		list_heap(&lambda, lambda, x, apply, NULL);
		list_heap(&x, setq, floop, lambda, NULL);
		loop_main_push_init(ptr, x);
		/* `(push ,form value-loop) */
		GetConst(COMMON_PUSH, &push);
		GetConst(SYSTEM_VALUE_LOOP, &value);
		list_heap(&x, push, form, value, NULL);
		loop_main_push_form(ptr, x);
	}
	else {
		/* `(setq ,into (append ,into ,form)) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(COMMON_APPEND, &append);
		list_heap(&append, append, into, form, NULL);
		list_heap(&x, setq, into, append, NULL);
		loop_main_push_form(ptr, x);
	}
}

static void loop_push_main_nconc(struct loop_main *ptr, addr list)
{
	addr form, into, value, setq, push, nconc, x;
	addr floop, lambda, apply, fsym, nreverse;

	list_bind(list, &form, &into, NULL);
	if (into == Unbound) {
		/* `(setq function-loop
		 *    (lambda (x)
		 *      (apply (function nconc) (nreverse x))))
		 */
		GetConst(COMMON_SETQ, &setq);
		GetConst(SYSTEM_FUNCTION_LOOP, &floop);
		GetConst(COMMON_LAMBDA, &lambda);
		GetConst(COMMON_APPLY, &apply);
		GetConst(COMMON_FUNCTION, &fsym);
		GetConst(COMMON_NCONC, &nconc);
		GetConst(COMMON_NREVERSE, &nreverse);
		make_symbolchar(&x, "X");
		list_heap(&nreverse, nreverse, x, NULL);
		list_heap(&fsym, fsym, nconc, NULL);
		list_heap(&apply, apply, fsym, nreverse, NULL);
		list_heap(&x, x, NULL);
		list_heap(&lambda, lambda, x, apply, NULL);
		list_heap(&x, setq, floop, lambda, NULL);
		loop_main_push_init(ptr, x);
		/* `(push ,form value-loop) */
		GetConst(COMMON_PUSH, &push);
		GetConst(SYSTEM_VALUE_LOOP, &value);
		list_heap(&x, push, form, value, NULL);
		loop_main_push_form(ptr, x);
	}
	else {
		/* `(setq ,into (nconc ,into ,form)) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(COMMON_NCONC, &nconc);
		list_heap(&nconc, nconc, into, form, NULL);
		list_heap(&x, setq, into, nconc, NULL);
		loop_main_push_form(ptr, x);
	}
}

static void loop_push_main_count(struct loop_main *ptr, addr list)
{
	addr form, into, type, value, setq, floop, ifsym, incf, x;

	list_bind(list, &form, &into, &type, NULL);
	if (into == Unbound) {
		/* `(setq value-loop 0) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(SYSTEM_VALUE_LOOP, &value);
		fixnum_heap(&x, 0);
		list_heap(&x, setq, value, x, NULL);
		loop_main_push_init(ptr, x);
		/* `(setq function-loop nil) */
		GetConst(SYSTEM_FUNCTION_LOOP, &floop);
		list_heap(&x, setq, floop, Nil, NULL);
		loop_main_push_init(ptr, x);
		/* `(if ,form (incf value-loop)) */
		GetConst(COMMON_IF, &ifsym);
		GetConst(COMMON_INCF, &incf);
		list_heap(&incf, incf, value, NULL);
		list_heap(&x, ifsym, form, incf, NULL);
		loop_main_push_form(ptr, x);
	}
	else {
		/* `(if ,form (incf ,into)) */
		GetConst(COMMON_IF, &ifsym);
		GetConst(COMMON_INCF, &incf);
		list_heap(&incf, incf, into, NULL);
		list_heap(&x, ifsym, form, incf, NULL);
		loop_main_push_form(ptr, x);
	}
}

static void loop_push_main_sum(struct loop_main *ptr, addr list)
{
	addr form, into, type, value, setq, floop, incf, x;

	list_bind(list, &form, &into, &type, NULL);
	if (into == Unbound) {
		/* `(setq value-loop 0) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(SYSTEM_VALUE_LOOP, &value);
		fixnum_heap(&x, 0);
		list_heap(&x, setq, value, x, NULL);
		loop_main_push_init(ptr, x);
		/* `(setq function-loop nil) */
		GetConst(SYSTEM_FUNCTION_LOOP, &floop);
		list_heap(&x, setq, floop, Nil, NULL);
		loop_main_push_init(ptr, x);
		/* `(incf value-loop ,form) */
		GetConst(COMMON_INCF, &incf);
		list_heap(&x, incf, value, form, NULL);
		loop_main_push_form(ptr, x);
	}
	else {
		/* `(incf ,into ,form) */
		GetConst(COMMON_INCF, &incf);
		list_heap(&x, incf, into, form, NULL);
		loop_main_push_form(ptr, x);
	}
}

static void loop_push_main_maxmin(struct loop_main *ptr, addr list, constindex index)
{
	addr form, into, type, value, setq, floop, let, ifsym, maxmin, x, y, z, g;

	list_bind(list, &form, &into, &type, NULL);
	if (into == Unbound) {
		/* `(setq value-loop nil) */
		GetConst(COMMON_SETQ, &setq);
		GetConst(SYSTEM_VALUE_LOOP, &value);
		list_heap(&x, setq, value, Nil, NULL);
		loop_main_push_init(ptr, x);
		/* `(setq function-loop nil) */
		GetConst(SYSTEM_FUNCTION_LOOP, &floop);
		list_heap(&x, setq, floop, Nil, NULL);
		loop_main_push_init(ptr, x);
	}
	else {
		value = into;
	}
	/* `(let ((g ,form))
	 *    (if value
	 *      (setq value (maxmin value ,g))
	 *      (setq value ,g)))
	 */
	make_symbolchar(&g, "G");
	GetConst(COMMON_LET, &let);
	GetConst(COMMON_IF, &ifsym);
	GetConst(COMMON_SETQ, &setq);
	GetConstant(index, &maxmin);
	list_heap(&form, g, form, NULL);
	list_heap(&form, form, NULL);
	list_heap(&maxmin, maxmin, value, g, NULL);
	list_heap(&y, setq, value, maxmin, NULL);
	list_heap(&z, setq, value, g, NULL);
	list_heap(&ifsym, ifsym, value, y, z, NULL);
	list_heap(&x, let, form, ifsym, NULL);
	loop_main_push_form(ptr, x);
}

static void loop_push_main_maximize(struct loop_main *ptr, addr list)
{
	loop_push_main_maxmin(ptr, list, CONSTANT_COMMON_MAX);
}

static void loop_push_main_minimize(struct loop_main *ptr, addr list)
{
	loop_push_main_maxmin(ptr, list, CONSTANT_COMMON_MIN);
}

static void loop_let_main_count(addr *form, addr list)
{
	addr value, into, type, let, zero;

	list_bind(list, &value, &into, &type, NULL);
	if (into != Unbound) {
		/* (let ((,into 0)) ,form) */
		GetConst(COMMON_LET, &let);
		fixnum_heap(&zero, 0);
		list_heap(&into, into, zero, NULL);
		list_heap(&into, into, NULL);
		list_heap(form, let, into, *form, NULL);
	}
}

static void loop_let_main_maximize(addr *form, addr list)
{
	addr value, into, type, let;

	list_bind(list, &value, &into, &type, NULL);
	if (into != Unbound) {
		/* (let (,into) ,form) */
		GetConst(COMMON_LET, &let);
		list_heap(&into, into, NULL);
		list_heap(form, let, into, *form, NULL);
	}
}

static void loop_let_main_repeat(addr *form, addr list)
{
	addr expr, a, b, let, zero;

	list_bind(list, &expr, &a, &b, NULL);
	/* (let ((,a 0) ,b) ,form) */
	GetConst(COMMON_LET, &let);
	fixnum_heap(&zero, 0);
	list_heap(&a, a, zero, NULL);
	list_heap(&a, a, b, NULL);
	list_heap(form, let, a, *form, NULL);
}

static void loop_push_main_if_unless(struct loop_main *ptr,
		addr list, constindex index)
{
	addr form, expr1, expr2, end_if, else_if, x;
	addr setq, it, if_unless, go;

	GetConstant(index, &if_unless);
	list_bind(list, &form, &expr1, &expr2, NULL);
	if (expr2 == Unbound) {
		/* (setq it-loop ,form)
		 * (if_unless it-loop (go #:end-if))
		 *   ,@expr1
		 * #:end-if
		 */
		make_symbolchar(&end_if, "END-IF");
		GetConst(COMMON_SETQ, &setq);
		GetConst(COMMON_GO, &go);
		GetConst(SYSTEM_IT_LOOP, &it);
		list_heap(&x, setq, it, form, NULL);
		loop_main_push_form(ptr, x);
		list_heap(&x, go, end_if, NULL);
		list_heap(&x, if_unless, it, x, NULL);
		loop_main_push_form(ptr, x);
		loop_push_main(ptr, expr1);
		loop_main_push_form(ptr, end_if);
	}
	else {
		/* (setq it-loop ,form)
		 * (if_unless it-loop (go #:else-if))
		 *   ,@expr1
		 * (go #:end-if)
		 * #:else-if
		 *   ,@expr2
		 * #:end-if
		 */
		make_symbolchar(&else_if, "ELSE-IF");
		make_symbolchar(&end_if, "END-IF");
		GetConst(COMMON_SETQ, &setq);
		GetConst(COMMON_GO, &go);
		GetConst(SYSTEM_IT_LOOP, &it);
		list_heap(&x, setq, it, form, NULL);
		loop_main_push_form(ptr, x);
		list_heap(&x, go, else_if, NULL);
		list_heap(&x, if_unless, it, x, NULL);
		loop_main_push_form(ptr, x);
		loop_push_main(ptr, expr1);
		list_heap(&x, go, end_if, NULL);
		loop_main_push_form(ptr, x);
		loop_main_push_form(ptr, else_if);
		loop_push_main(ptr, expr2);
		loop_main_push_form(ptr, end_if);
	}
}

static void loop_push_main_if(struct loop_main *ptr, addr list)
{
	loop_push_main_if_unless(ptr, list, CONSTANT_COMMON_UNLESS);
}

static void loop_push_main_unless(struct loop_main *ptr, addr list)
{
	loop_push_main_if_unless(ptr, list, CONSTANT_COMMON_IF);
}

static void loop_let_if_unless(addr *form, addr list)
{
	addr expr, expr1, expr2;

	list_bind(list, &expr, &expr1, &expr2, NULL);
	loop_let_main(form, expr1);
	if (expr2 != Unbound)
		loop_let_main(form, expr2);
}

static void loop_push_main_while_until(struct loop_main *ptr,
		addr list, constindex index)
{
	addr expr, check, go, end_loop;

	list_bind(list, &expr, NULL);
	/* `(unless ,expr (go end-loop)) */
	GetConstant(index, &check);
	GetConst(COMMON_GO, &go);
	GetConst(SYSTEM_END_LOOP, &end_loop);
	list_heap(&go, go, end_loop, NULL);
	list_heap(&check, check, expr, go, NULL);
	loop_main_push_form(ptr, check);
}

static void loop_push_main_while(struct loop_main *ptr, addr list)
{
	loop_push_main_while_until(ptr, list, CONSTANT_COMMON_UNLESS);
}

static void loop_push_main_until(struct loop_main *ptr, addr list)
{
	loop_push_main_while_until(ptr, list, CONSTANT_COMMON_WHEN);
}

static void loop_push_main_terminate(struct loop_main *ptr,
		addr list, constindex index)
{
	addr expr, setq, value, check, retfrom, x;

	list_bind(list, &expr, NULL);
	/* `(setq value-loop t) */
	GetConst(COMMON_SETQ, &setq);
	GetConst(SYSTEM_VALUE_LOOP, &value);
	list_heap(&x, setq, value, T, NULL);
	loop_main_push_init(ptr, x);
	/* `(unless ,expr (return-from named nil)) */
	GetConstant(index, &check);
	GetConst(COMMON_RETURN_FROM, &retfrom);
	list_heap(&retfrom, retfrom, ptr->named, Nil, NULL);
	list_heap(&x, check, expr, retfrom, NULL);
	loop_main_push_form(ptr, x);
}

static void loop_push_main_always(struct loop_main *ptr, addr list)
{
	loop_push_main_terminate(ptr, list, CONSTANT_COMMON_UNLESS);
}

static void loop_push_main_never(struct loop_main *ptr, addr list)
{
	loop_push_main_terminate(ptr, list, CONSTANT_COMMON_WHEN);
}

static void loop_push_main_thereis(struct loop_main *ptr, addr list)
{
	addr expr, setq, value, let, ifsym, retfrom, x, g;

	list_bind(list, &expr, NULL);
	/* `(setq value-loop nil) */
	GetConst(COMMON_SETQ, &setq);
	GetConst(SYSTEM_VALUE_LOOP, &value);
	list_heap(&x, setq, value, Nil, NULL);
	loop_main_push_init(ptr, x);
	/* `(let ((,g ,expr))
	 *    (if ,g (return-from named ,g)))
	 */
	GetConst(COMMON_LET, &let);
	GetConst(COMMON_IF, &ifsym);
	GetConst(COMMON_RETURN_FROM, &retfrom);
	make_symbolchar(&g, "G");
	list_heap(&retfrom, retfrom, ptr->named, g, NULL);
	list_heap(&ifsym, ifsym, g, retfrom, NULL);
	list_heap(&g, g, expr, NULL);
	list_heap(&g, g, NULL);
	list_heap(&x, let, g, ifsym, NULL);
	loop_main_push_form(ptr, x);
}

static void loop_push_main_repeat(struct loop_main *ptr, addr list)
{
	addr expr, a, b, setq, ifsym, less, incf, go, end_loop, x;

	list_bind(list, &expr, &a, &b, NULL);
	/* `(setq ,b ,expr) */
	GetConst(COMMON_SETQ, &setq);
	list_heap(&x, setq, b, expr, NULL);
	loop_main_push_init(ptr, x);
	/* `(if (< ,a ,b)
	 *    (incf ,a)
	 *    (go end-loop))
	 */
	GetConst(COMMON_IF, &ifsym);
	GetConst(COMMON_NUMBER_LESS, &less);
	GetConst(COMMON_INCF, &incf);
	GetConst(COMMON_GO, &go);
	GetConst(SYSTEM_END_LOOP, &end_loop);
	list_heap(&go, go, end_loop, NULL);
	list_heap(&incf, incf, a, NULL);
	list_heap(&less, less, a, b, NULL);
	list_heap(&x, ifsym, less, incf, go, NULL);
	loop_main_push_form(ptr, x);
}

_g void loop_push_main(struct loop_main *ptr, addr list)
{
	addr car, cdr;
	addr do_p, return_p, collect_p, append_p, nconc_p;
	addr count_p, sum_p, max_p, min_p, if_p, unless_p;
	addr while_p, until_p, always_p, never_p, thereis_p, repeat_p;

	GetConst(SYSTEM_LOOP_DO, &do_p);
	GetConst(SYSTEM_LOOP_RETURN, &return_p);
	GetConst(SYSTEM_LOOP_IF, &if_p);
	GetConst(SYSTEM_LOOP_UNLESS, &unless_p);
	GetConst(SYSTEM_LOOP_COLLECT, &collect_p);
	GetConst(SYSTEM_LOOP_APPEND, &append_p);
	GetConst(SYSTEM_LOOP_NCONC, &nconc_p);
	GetConst(SYSTEM_LOOP_COUNT, &count_p);
	GetConst(SYSTEM_LOOP_SUM, &sum_p);
	GetConst(SYSTEM_LOOP_MAXIMIZE, &max_p);
	GetConst(SYSTEM_LOOP_MINIMIZE, &min_p);
	GetConst(SYSTEM_LOOP_WHILE, &while_p);
	GetConst(SYSTEM_LOOP_UNTIL, &until_p);
	GetConst(SYSTEM_LOOP_ALWAYS, &always_p);
	GetConst(SYSTEM_LOOP_NEVER, &never_p);
	GetConst(SYSTEM_LOOP_THEREIS, &thereis_p);
	GetConst(SYSTEM_LOOP_REPEAT, &repeat_p);
	while (list != Nil) {
		GetCons(list, &cdr, &list);
		GetCons(cdr, &car, &cdr);
		/* conditional */
		if (car == if_p) {
			loop_push_main_if(ptr, cdr);
			continue;
		}
		if (car == unless_p) {
			loop_push_main_unless(ptr, cdr);
			continue;
		}
		/* unconditional */
		if (car == do_p) {
			loop_push_main_do(ptr, cdr);
			continue;
		}
		if (car == return_p) {
			loop_push_main_return(ptr, cdr);
			continue;
		}
		/* accumulation */
		if (car == collect_p) {
			loop_push_main_collect(ptr, cdr);
			continue;
		}
		if (car == append_p) {
			loop_push_main_append(ptr, cdr);
			continue;
		}
		if (car == nconc_p) {
			loop_push_main_nconc(ptr, cdr);
			continue;
		}
		if (car == count_p) {
			loop_push_main_count(ptr, cdr);
			continue;
		}
		if (car == sum_p) {
			loop_push_main_sum(ptr, cdr);
			continue;
		}
		if (car == max_p) {
			loop_push_main_maximize(ptr, cdr);
			continue;
		}
		if (car == min_p) {
			loop_push_main_minimize(ptr, cdr);
			continue;
		}
		/* termination */
		if (car == while_p) {
			loop_push_main_while(ptr, cdr);
			continue;
		}
		if (car == until_p) {
			loop_push_main_until(ptr, cdr);
			continue;
		}
		if (car == always_p) {
			loop_push_main_always(ptr, cdr);
			continue;
		}
		if (car == never_p) {
			loop_push_main_never(ptr, cdr);
			continue;
		}
		if (car == thereis_p) {
			loop_push_main_thereis(ptr, cdr);
			continue;
		}
		if (car == repeat_p) {
			loop_push_main_repeat(ptr, cdr);
			continue;
		}
		/* error */
		fmte("Invalid loop operator ~S.", car, NULL);
	}
}

_g void loop_let_main(addr *form, addr list)
{
	addr car, cdr;
	addr do_p, return_p, collect_p, append_p, nconc_p;
	addr count_p, sum_p, max_p, min_p, if_p, unless_p;
	addr while_p, until_p, always_p, never_p, thereis_p, repeat_p;

	GetConst(SYSTEM_LOOP_DO, &do_p);
	GetConst(SYSTEM_LOOP_RETURN, &return_p);
	GetConst(SYSTEM_LOOP_IF, &if_p);
	GetConst(SYSTEM_LOOP_UNLESS, &unless_p);
	GetConst(SYSTEM_LOOP_COLLECT, &collect_p);
	GetConst(SYSTEM_LOOP_APPEND, &append_p);
	GetConst(SYSTEM_LOOP_NCONC, &nconc_p);
	GetConst(SYSTEM_LOOP_COUNT, &count_p);
	GetConst(SYSTEM_LOOP_SUM, &sum_p);
	GetConst(SYSTEM_LOOP_MAXIMIZE, &max_p);
	GetConst(SYSTEM_LOOP_MINIMIZE, &min_p);
	GetConst(SYSTEM_LOOP_WHILE, &while_p);
	GetConst(SYSTEM_LOOP_UNTIL, &until_p);
	GetConst(SYSTEM_LOOP_ALWAYS, &always_p);
	GetConst(SYSTEM_LOOP_NEVER, &never_p);
	GetConst(SYSTEM_LOOP_THEREIS, &thereis_p);
	GetConst(SYSTEM_LOOP_REPEAT, &repeat_p);
	while (list != Nil) {
		GetCons(list, &cdr, &list);
		GetCons(cdr, &car, &cdr);
		/* conditional */
		if (car == if_p) {
			loop_let_if_unless(form, cdr);
			continue;
		}
		if (car == unless_p) {
			loop_let_if_unless(form, cdr);
			continue;
		}
		/* unconditional */
		if (car == do_p) {
			continue;
		}
		if (car == return_p) {
			continue;
		}
		/* list-accumulation */
		if (car == collect_p || car == append_p || car == nconc_p) {
			loop_let_main_collect(form, cdr);
			continue;
		}
		/* numeric-accumulation */
		if (car == count_p || car == sum_p) {
			loop_let_main_count(form, cdr);
			continue;
		}
		if (car == max_p || car == min_p) {
			loop_let_main_maximize(form, cdr);
			continue;
		}
		if (car == while_p || car == until_p ||
				car == always_p || car == never_p || car == thereis_p) {
			continue;
		}
		if (car == repeat_p) {
			loop_let_main_repeat(form, cdr);
			continue;
		}
		/* error */
		fmte("Invalid loop operator ~S.", car, NULL);
	}
}

