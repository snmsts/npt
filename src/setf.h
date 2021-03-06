#ifndef __SETF_HEADER__
#define __SETF_HEADER__

#include "typedef.h"

/* common-lisp setf-expansion */
_g void function_setf_values(Execute ptr, addr args, addr env);
_g void function_setf_getf(Execute ptr, addr args, addr env);

/* get-setf-expander */
_g int get_setf_expansion(Execute ptr, addr form, addr env,
		addr *vars, addr *vals, addr *store, addr *writer, addr *reader);

#endif

