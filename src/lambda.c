#include "build.h"
#include "cons.h"
#include "constant.h"
#include "condition.h"
#include "lambda.h"
#include "package.h"
#include "sequence.h"
#include "symbol.h"

/*
 *  parse
 */
enum AMPERSAND_ARGUMENT {
	AMPERSAND_GENERIC,
	AMPERSAND_ORDINARY,
	AMPERSAND_METHOD_COMBINATION,
	AMPERSAND_MACRO,
	AMPERSAND_SIZE
};

static int constant_eq(constindex index, addr pos)
{
	addr check;
	GetConstant(index, &check);
	return check == pos;
}

static int member_ampersand(addr pos, enum AMPERSAND_ARGUMENT ampersand)
{
	/* generic */
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, pos)) return 1;
	if (constant_eq(CONSTANT_AMPERSAND_REST, pos)) return 1;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, pos)) return 1;
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, pos)) return 1;
	if (ampersand <= AMPERSAND_GENERIC) return 0;
	/* ordinary */
	if (constant_eq(CONSTANT_AMPERSAND_AUX, pos)) return 1;
	if (ampersand <= AMPERSAND_ORDINARY) return 0;
	/* method-combination */
	if (constant_eq(CONSTANT_AMPERSAND_WHOLE, pos)) return 1;
	if (ampersand <= AMPERSAND_METHOD_COMBINATION) return 0;
	/* macro */
	if (constant_eq(CONSTANT_AMPERSAND_BODY, pos)) return 1;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, pos)) return 1;

	return 0;
}

static void variable_check(addr var, enum AMPERSAND_ARGUMENT ampersand)
{
	if (var == Nil || var == T)
		fmte("The symbol ~S must not be a constant symbol.", var, NULL);
	if (! IsSymbol(var))
		fmte("The variable ~S is not a symbol.", var, NULL);
	if (GetStatusReadOnly(var))
		fmte("The symbol ~S must not be a constant symbol.", var, NULL);
	if (member_ampersand(var, ampersand))
		fmte("The symbol ~S don't use in the lambda-list.", var, NULL);
}

/* push-varcons */
static void varcons_local(LocalRoot local, addr *ret)
{
	queue_local(local, ret);
}

static void varcons_data(addr instance, addr *ret)
{
	rootqueue(instance, ret);
}

static void push_varcons(LocalRoot local,
		addr instance, addr var, enum AMPERSAND_ARGUMENT ampersand)
{
	addr data;

	variable_check(var, ampersand);
	varcons_data(instance, &data);
	if (find_list_eq_unsafe(var, data))
		fmte("The variable ~S is already used.", var, NULL);
	pushqueue_local(local, instance, var);
}

static void push_namecons(LocalRoot local, addr instance, addr name)
{
	addr data;

	varcons_data(instance, &data);
	if (find_list_eq_unsafe(name, data))
		fmte("The key name ~S is already used.", name, NULL);
	pushqueue_local(local, instance, name);
}

static void make_keyword_from_symbol(addr symbol, addr *ret)
{
	addr keyword, package;

	if (! IsSymbol(symbol))
		fmte("The variable ~S must be a symbol.", symbol, NULL);
	GetConst(PACKAGE_KEYWORD, &keyword);
	GetPackageSymbol(symbol, &package);
	if (package == keyword) {
		*ret = symbol;
	}
	else {
		GetNameSymbol(symbol, &symbol);
		intern_package(keyword, symbol, ret);
	}
}

static int list2_check(addr cons, addr *ret1, addr *ret2)
{
	addr pos1, pos2;

	if (GetType(cons) != LISPTYPE_CONS) return 1;
	GetCons(cons, &pos1, &cons);
	if (GetType(cons) != LISPTYPE_CONS) return 1;
	GetCons(cons, &pos2, &cons);
	if (cons != Nil) return 1;
	*ret1 = pos1;
	*ret2 = pos2;

	return 0;
}

static void key_name_values(addr pos, addr *symbol, addr *name)
{
	if (GetType(pos) == LISPTYPE_CONS) {
		/* swap (name, symbol) -> (symbol, name). */
		if (list2_check(pos, name, symbol)) {
			fmte("The variable ~S must be a symbol or (name symbol) list.", pos, NULL);
			*symbol = *name = NULL;
		}
	}
	else {
		*symbol = pos;
		make_keyword_from_symbol(pos, name);
	}
}


/*
 *  lambda-macro
 */
static void push_varcons_macro(LocalRoot local, addr instance, addr key)
{
	push_varcons(local, instance, key, AMPERSAND_MACRO);
}

#define nextcons_finish_rest(one, cons) { \
	GetCdr(cons, &cons); \
	if (cons == Nil) goto finish; \
	if (! consp(cons)) goto finish_rest; \
	GetCar(cons, &one); \
}

#define nextcons_finish_error(one, cons, name) { \
	GetCdr(cons, &cons); \
	if (cons == Nil) goto finish; \
	if (! consp(cons)) { \
		addr __symbol; \
		GetConst(AMPERSAND_##name, &__symbol); \
		fmte("After ~A parameter don't allow a dot list.", __symbol, NULL); \
	} \
	GetCar(cons, &one); \
}

#define environment_expander(local, instance, envcheck, env, one, cons) { \
	if (! envcheck) { \
		fmte("&environment don't accept at no top-level argument.", NULL); \
	} \
	if (env != Nil) { \
		fmte("&environment must be at once in arguments.", NULL); \
	} \
	GetCdr(cons, &cons); \
	if (cons == Nil) { \
		fmte("&environment parameter must be a one argument.", NULL); \
	} \
	if (! consp(cons)) { \
		fmte("After &environment don't allow a dot list.", NULL); \
	} \
	GetCar(cons, &one); \
	push_varcons_macro(local, instance, one); \
	env = one; \
}


static void ordinary_opt_default(addr cons, addr init,
		addr *retvar, addr *retinit, addr *retsup)
{
	addr base, var, sup;

	base = cons;
	var = sup = Nil;
	/* var */
	if (GetType(cons) != LISPTYPE_CONS) {
		var = cons;
		goto final;
	}
	/* (var) */
	GetCons(cons, &var, &cons);
	if (cons == Nil) goto final;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	/* (var init) */
	GetCons(cons, &init, &cons);
	if (cons == Nil) goto final;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	/* (var init sup) */
	GetCons(cons, &sup, &cons);
	if (sup == Nil)
		fmte("The supplied variable ~S must not be a constant symbol.", sup, NULL);
	if (cons == Nil) goto final;
	/* (var init sup . error) */
	fmte("Too many argument ~S.", base, NULL);

final:
	*retvar = var;
	*retinit = init;
	*retsup = sup;
}

static void ordinary_opt(addr cons, addr *retvar, addr *retinit, addr *retsup)
{
	ordinary_opt_default(cons, Nil, retvar, retinit, retsup);
}

static void ordinary_key_default(LocalRoot local, addr name, addr cons, addr value,
		addr *retvar, addr *retname, addr *retinit, addr *retsup)
{
	ordinary_opt_default(cons, value, &cons, retinit, retsup);
	key_name_values(cons, retvar, retname);
	push_namecons(local, name, *retname);
}

static void ordinary_key(LocalRoot local, addr name, addr cons,
		addr *retvar, addr *retname, addr *retinit, addr *retsup)
{
	ordinary_key_default(local, name, cons, Nil, retvar, retname, retinit, retsup);
}

static void ordinary_aux(addr cons, addr *retvar, addr *retinit)
{
	addr base, var, init;

	base = cons;
	var = init = Nil;
	/* var */
	if (GetType(cons) != LISPTYPE_CONS) {
		var = cons;
		goto final;
	}
	/* (var) */
	GetCons(cons, &var, &cons);
	if (cons == Nil) goto final;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	/* (var init) */
	GetCons(cons, &init, &cons);
	if (cons == Nil) goto final;
	/* (var init . error) */
	fmte("Too many argument ~S.", base, NULL);

final:
	*retvar = var;
	*retinit = init;
}

static void lambda_macro_call(LocalRoot local,
		addr *ret, addr cons, addr instance, addr defvalue)
{
	int envcheck;
	addr name, var, opt, rest, key, key_p, allow, aux, one;
	addr whole, env, temp, symbol, init, sup;

	var = opt = rest = key = key_p = allow = aux = whole = env = Nil;
	envcheck = (instance == Nil);
	if (envcheck)
		varcons_local(local, &instance);
	varcons_local(local, &name);
	if (cons == Nil) goto finish;
	if (! consp(cons)) goto finish_rest;
	GetCar(cons, &one);
	if (constant_eq(CONSTANT_AMPERSAND_WHOLE, one)) goto whole_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto var_environment;
	goto var_argument;

whole_argument:
	GetCdr(cons, &cons);
	if (cons == Nil)
		fmte("&whole parameter must be a one argument.", NULL);
	if (! consp(cons))
		fmte("After &whole parameter don't allow a dot list.", NULL);
	GetCar(cons, &one);
	push_varcons_macro(local, instance, one);
	whole = one;
	nextcons_finish_rest(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto var_environment;
	goto var_argument;

var_environment:
	environment_expander(local, instance, envcheck, env, one, cons);
	nextcons_finish_rest(one, cons);

var_argument:
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_BODY, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto optional_environment;
	if (listp(one))
		lambda_macro_call(local, &one, one, instance, defvalue);
	else
		push_varcons_macro(local, instance, one);
	cons_heap(&var, one, var);
	nextcons_finish_rest(one, cons);
	goto var_argument;

optional_environment:
	environment_expander(local, instance, envcheck, env, one, cons);
	nextcons_finish_rest(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_BODY, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("Invalid ~A argument.", one, NULL);

optional_argument:
	nextcons_finish_rest(one, cons);
optional_loop:
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_BODY, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto rest_environment;
	ordinary_opt_default(one, defvalue, &symbol, &init, &sup);
	push_varcons_macro(local, instance, symbol);
	if (sup != Nil)
		push_varcons_macro(local, instance, sup);
	list_heap(&one, symbol, init, sup, NULL);
	cons_heap(&opt, one, opt);
	nextcons_finish_rest(one, cons);
	goto optional_loop;

rest_environment:
	environment_expander(local, instance, envcheck, env, one, cons);
	nextcons_finish_rest(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_BODY, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("Invalid ~A argument.", one, NULL);

rest_argument:
	GetCdr(cons, &cons);
	if (cons == Nil)
		fmte("&rest parameter must be a one argument.", NULL);
	if (! consp(cons))
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &temp);
	push_varcons_macro(local, instance, temp);
	cons_heap(&rest, temp, one);

	nextcons_finish_error(one, cons, REST);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto key_environment;
	fmte("After &rest/&body ~S must be a null or &key arguments.", one, NULL);

key_environment:
	environment_expander(local, instance, envcheck, env, one, cons);
	nextcons_finish_error(one, cons, REST);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("Invalid ~A argument.", one, NULL);

key_argument:
	key_p = T;
	nextcons_finish_error(one, cons, KEY);
key_loop:
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, one)) goto allow_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto aux_environment;
	ordinary_key_default(local, name, one, defvalue, &symbol, &one, &init, &sup);
	push_varcons_macro(local, instance, symbol);
	if (sup != Nil)
		push_varcons_macro(local, instance, sup);
	list_heap(&one, symbol, one, init, sup, NULL);
	cons_heap(&key, one, key);
	nextcons_finish_error(one, cons, KEY);
	goto key_loop;

allow_argument:
	allow = T;
	nextcons_finish_error(one, cons, ALLOW);
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto aux_environment;
	fmte("Invalid ~A argument.", one, NULL);

aux_environment:
	environment_expander(local, instance, envcheck, env, one, cons);
	nextcons_finish_error(one, cons, KEY);
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("Invalid ~A argument.", one, NULL);

aux_argument:
	nextcons_finish_error(one, cons, AUX);
aux_loop:
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto finish_environment;
	ordinary_aux(one, &symbol, &init);
	push_varcons_macro(local, instance, symbol);
	list_heap(&one, symbol, init, NULL);
	cons_heap(&aux, one, aux);
	nextcons_finish_error(one, cons, AUX);
	goto aux_loop;

finish_environment:
	environment_expander(local, instance, envcheck, env, one, cons);
	nextcons_finish_error(one, cons, AUX);
	fmte("Invalid ~A argument.", one, NULL);

finish_rest:
	push_varcons_macro(local, instance, cons);
	conscar_heap(&rest, cons);
	goto finish;

finish:
	list_heap(ret,
			nreverse_list_unsafe_inplace(var),
			nreverse_list_unsafe_inplace(opt),
			rest,  /* (var . &rest) (var . &body) (var . nil) */
			key != Nil? nreverse_list_unsafe_inplace(key): key_p,
			allow,
			nreverse_list_unsafe_inplace(aux),
			whole,
			env,
			NULL);
}

_g void lambda_macro(LocalRoot local, addr *ret, addr cons, addr instance)
{
	lambda_macro_call(local, ret, cons, instance, Nil);
}


/*
 *  lambda-ordinary
 */
_g void lambda_deftype(LocalRoot local, addr *ret, addr cons, addr instance)
{
	addr aster;

	GetConst(COMMON_ASTERISK, &aster);
	lambda_macro_call(local, ret, cons, instance, aster);
}


/*
 *  lambda-ordinary
 */
static void push_varcons_ordinary(LocalRoot local, addr instance, addr key)
{
	push_varcons(local, instance, key, AMPERSAND_ORDINARY);
}


/*
 *  lambda-generic-function
 */
static void generic_function_key_cons(addr cons, addr *symbol, addr *name)
{
	addr var;

	/* var */
	if (GetType(cons) != LISPTYPE_CONS) {
		var = cons;
		goto final;
	}
	/* (var) */
	GetCons(cons, &var, &cons);
	if (cons == Nil) {
		goto final;
	}
	/* (var . error) */
	fmte("&key parameter ~S must be at many one arguments.", cons, NULL);

final:
	key_name_values(var, symbol, name);
}

static void generic_function_key(LocalRoot local,
		addr instance, addr cons, addr *var, addr *name)
{
	generic_function_key_cons(cons, var, name);
	push_namecons(local, instance, *name);
}

static void push_varcons_generic_function(LocalRoot local,
		addr instance, addr key)
{
	push_varcons(local, instance, key, AMPERSAND_GENERIC);
}

#define nextcons_finish(one, cons) { \
	GetCdr(cons, &cons); \
	if (cons == Nil) goto finish; \
	if (GetType(cons) != LISPTYPE_CONS) { \
		fmte("Dot list don't allow in this lambda-list.", NULL); \
	} \
	GetCar(cons, &one); \
}

_g void lambda_generic_function(LocalRoot local, addr *ret, addr cons)
{
	addr instance, name, var, opt, rest, key, key_p, allow, one, symbol;

	var = opt = rest = key = key_p = allow = Nil;
	varcons_local(local, &instance);
	varcons_local(local, &name);
	if (cons == Nil) goto finish;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &one);

var_argument:
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	push_varcons_generic_function(local, instance, one);
	cons_heap(&var, one, var);
	nextcons_finish(one, cons);
	goto var_argument;

optional_argument:
	nextcons_finish(one, cons);
optional_loop:
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (singlep(one))
		GetCar(one, &one);
	push_varcons_generic_function(local, instance, one);
	cons_heap(&opt, one, opt);
	nextcons_finish(one, cons);
	goto optional_loop;

rest_argument:
	GetCdr(cons, &cons);
	if (cons == Nil)
		fmte("&rest parameter must be a one argument.", NULL);
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &rest);
	push_varcons_generic_function(local, instance, rest);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	fmte("After &rest ~S must be a null or &key arguments.", cons, NULL);

key_argument:
	key_p = T;
	nextcons_finish(one, cons);
key_loop:
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, one)) goto allow_argument;
	generic_function_key(local, name, one, &symbol, &one);
	push_varcons_generic_function(local, instance, symbol);
	list_heap(&one, symbol, one, NULL);
	cons_heap(&key, one, key);
	nextcons_finish(one, cons);
	goto key_loop;

allow_argument:
	allow = T;
	nextcons_finish(one, cons);
	fmte("After &allow-other-keys ~S must be a null.", cons, NULL);

finish:
	list_heap(ret,
			nreverse_list_unsafe_inplace(var),
			nreverse_list_unsafe_inplace(opt),
			rest,
			key != Nil? nreverse_list_unsafe_inplace(key): key_p,
			allow,
			NULL);
}

_g void atleast_argument_count(addr cons, size_t *ret)
{
	GetCar(cons, &cons);
	*ret = length_list_unsafe(cons);
}


/*
 *  lambda-specialized
 */
static void specialized_var_cons(addr cons, addr *retvar, addr *retspec)
{
	addr var, spec;

	spec = T;
	/* var */
	if (GetType(cons) != LISPTYPE_CONS) {
		var = cons;
		goto final;
	}
	/* (var) */
	GetCons(cons, &var, &cons);
	if (cons == Nil) goto final;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in the argument ~S.", cons, NULL);
	/* (var specializer) */
	GetCons(cons, &spec, &cons);
	if (cons == Nil) goto final;
	/* (var specializer . error) */
	fmte("The variable parameter must be at many two arguments.", NULL);

final:
	*retvar = var;
	*retspec = spec;
}

static int check_specializer_form(addr spec)
{
	enum LISPTYPE type;
	addr left, check;

	/* symbol */
	if (spec == Nil || spec == T) return 1;
	type = GetType(spec);
	if (type == LISPTYPE_SYMBOL) return 1;
	/* (eql spec) */
	if (type != LISPTYPE_CONS) return 0;
	GetCons(spec, &left, &spec);
	GetConst(COMMON_EQL, &check);
	if (left != check) return 0;
	if (GetType(spec) != LISPTYPE_CONS) return 0;
	GetCdr(spec, &spec);

	return spec == Nil;
}

static void specialized_var(addr cons, addr *var, addr *spec)
{
	specialized_var_cons(cons, var, spec);
	if (! check_specializer_form(*spec))
		fmte("The parameter ~S don't allow a specializer form.", *spec, NULL);
}

_g void lambda_specialized(LocalRoot local, addr *ret, addr cons)
{
	addr instance, name, var, opt, rest, key, key_p, allow, aux, one;
	addr symbol, init, sup;

	var = opt = rest = key = key_p = allow = aux = Nil;
	varcons_local(local, &instance);
	varcons_local(local, &name);
	if (cons == Nil) goto finish;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &one);

var_argument:
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	specialized_var(one, &symbol, &one);
	push_varcons_ordinary(local, instance, symbol);
	list_heap(&one, symbol, one, NULL);
	cons_heap(&var, one, var);
	nextcons_finish(one, cons);
	goto var_argument;

optional_argument:
	nextcons_finish(one, cons);
optional_loop:
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	ordinary_opt(one, &symbol, &init, &sup);
	push_varcons_ordinary(local, instance, symbol);
	if (sup != Nil)
		push_varcons_ordinary(local, instance, sup);
	list_heap(&one, symbol, init, sup, NULL);
	cons_heap(&opt, one, opt);
	nextcons_finish(one, cons);
	goto optional_loop;

rest_argument:
	GetCdr(cons, &cons);
	if (cons == Nil)
		fmte("&rest parameter must be a one argument.", NULL);
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &rest);
	push_varcons_ordinary(local, instance, rest);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("After &rest ~S must be a null or &key arguments.", cons, NULL);

key_argument:
	key_p = T;
	nextcons_finish(one, cons);
key_loop:
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, one)) goto allow_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	ordinary_key(local, name, one, &symbol, &one, &init, &sup);
	push_varcons_ordinary(local, instance, symbol);
	if (sup != Nil)
		push_varcons_ordinary(local, instance, sup);
	list_heap(&one, symbol, one, init, sup, NULL);
	cons_heap(&key, one, key);
	nextcons_finish(one, cons);
	goto key_loop;

allow_argument:
	allow = T;
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("After &allow-other-keys ~S must be a null.", cons, NULL);

aux_argument:
	nextcons_finish(one, cons);
aux_loop:
	ordinary_aux(one, &symbol, &init);
	push_varcons_ordinary(local, instance, symbol);
	list_heap(&one, symbol, init, NULL);
	cons_heap(&aux, one, aux);
	nextcons_finish(one, cons);
	goto aux_loop;

finish:
	list_heap(ret,
			nreverse_list_unsafe_inplace(var),
			nreverse_list_unsafe_inplace(opt),
			rest,
			key != Nil? nreverse_list_unsafe_inplace(key): key_p,
			allow,
			nreverse_list_unsafe_inplace(aux),
			NULL);
}


/*
 *  lambda_ordinary
 */
static void lambda_ordinary_g(LocalRoot local, addr *ret, addr cons, addr g)
{
	addr instance, name, var, opt, rest, key, key_p, allow, aux, one;
	addr symbol, init, sup;

	var = opt = rest = key = key_p = allow = aux = Nil;
	varcons_local(local, &instance);
	varcons_local(local, &name);
	if (cons == Nil) goto finish;
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &one);

var_argument:
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	push_varcons_ordinary(local, instance, one);
	cons_heap(&var, one, var);
	nextcons_finish(one, cons);
	goto var_argument;

optional_argument:
	nextcons_finish(one, cons);
optional_loop:
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	ordinary_opt_default(one, g, &symbol, &init, &sup);
	push_varcons_ordinary(local, instance, symbol);
	if (sup != Nil)
		push_varcons_ordinary(local, instance, sup);
	list_heap(&one, symbol, init, sup, NULL);
	cons_heap(&opt, one, opt);
	nextcons_finish(one, cons);
	goto optional_loop;

rest_argument:
	GetCdr(cons, &cons);
	if (cons == Nil)
		fmte("&rest parameter must be a one argument.", NULL);
	if (GetType(cons) != LISPTYPE_CONS)
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &rest);
	push_varcons_ordinary(local, instance, rest);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("After &rest ~S must be a null or &key arguments.", cons, NULL);

key_argument:
	key_p = T;
	nextcons_finish(one, cons);
key_loop:
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, one)) goto allow_argument;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	ordinary_key_default(local, name, one, g, &symbol, &one, &init, &sup);
	push_varcons_ordinary(local, instance, symbol);
	if (sup != Nil)
		push_varcons_ordinary(local, instance, sup);
	list_heap(&one, symbol, one, init, sup, NULL);
	cons_heap(&key, one, key);
	nextcons_finish(one, cons);
	goto key_loop;

allow_argument:
	allow = T;
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_AUX, one)) goto aux_argument;
	fmte("After &allow-other-keys ~S must be a null.", cons, NULL);

aux_argument:
	nextcons_finish(one, cons);
aux_loop:
	ordinary_aux(one, &symbol, &init);
	push_varcons_ordinary(local, instance, symbol);
	list_heap(&one, symbol, init, NULL);
	cons_heap(&aux, one, aux);
	nextcons_finish(one, cons);
	goto aux_loop;

finish:
	list_heap(ret,
			nreverse_list_unsafe_inplace(var),
			nreverse_list_unsafe_inplace(opt),
			rest,
			key != Nil? nreverse_list_unsafe_inplace(key): key_p,
			allow,
			nreverse_list_unsafe_inplace(aux),
			NULL);
}

_g void lambda_ordinary(LocalRoot local, addr *ret, addr cons)
{
	lambda_ordinary_g(local, ret, cons, Nil);
}


/*
 *  lambda_defsetf
 */
static void variable_check_defsetf(addr var)
{
	if (var == Nil || var == T)
		fmte("The symbol ~S must not be a constant symbol.", var, NULL);
	if (! IsSymbol(var))
		fmte("The variable ~S is not a symbol.", var, NULL);
	if (GetStatusReadOnly(var))
		fmte("The symbol ~S must not be a constant symbol.", var, NULL);
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, var)) return;
	if (constant_eq(CONSTANT_AMPERSAND_REST, var)) return;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, var)) return;
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, var)) return;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, var)) return;
	if (constant_eq(CONSTANT_AMPERSAND_AUX, var)) goto error;
	if (constant_eq(CONSTANT_AMPERSAND_WHOLE, var)) goto error;
	if (constant_eq(CONSTANT_AMPERSAND_BODY, var)) goto error;
	return;

error:
	fmte("The symbol ~S don't use in defsetf.", var, NULL);
}

static void push_varcons_defsetf(LocalRoot local, addr instance, addr var)
{
	addr data;

	variable_check_defsetf(var);
	varcons_data(instance, &data);
	if (find_list_eq_unsafe(var, data))
		fmte("The variable ~S is already used.", var, NULL);
	pushqueue_local(local, instance, var);
}

#define environment_expander_defsetf(local, instance, env, one, cons) { \
	if (env != Nil) { \
		fmte("&environment must be at once in arguments.", NULL); \
	} \
	GetCdr(cons, &cons); \
	if (cons == Nil) { \
		fmte("&environment parameter must be a one argument.", NULL); \
	} \
	if (! consp(cons)) { \
		fmte("After &environment don't allow a dot list.", NULL); \
	} \
	GetCar(cons, &one); \
	push_varcons_defsetf(local, instance, one); \
	env = one; \
}

_g void lambda_defsetf(LocalRoot local, addr *ret, addr cons)
{
	addr instance, name, var, opt, rest, key, key_p, allow, one;
	addr env, symbol, init, sup;

	var = opt = rest = key = key_p = allow = env = Nil;
	varcons_local(local, &instance);
	varcons_local(local, &name);
	if (cons == Nil) goto finish;
	if (! consp(cons))
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &one);
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto var_environment;
	goto var_argument;

var_environment:
	environment_expander_defsetf(local, instance, env, one, cons);
	nextcons_finish(one, cons);

var_argument:
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto optional_environment;
	push_varcons_defsetf(local, instance, one);
	cons_heap(&var, one, var);
	nextcons_finish(one, cons);
	goto var_argument;

optional_environment:
	environment_expander_defsetf(local, instance, env, one, cons);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_OPTIONAL, one)) goto optional_argument;
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	fmte("Invalid ~A argument.", one, NULL);

optional_argument:
	nextcons_finish(one, cons);
optional_loop:
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto rest_environment;
	ordinary_opt(one, &symbol, &init, &sup);
	push_varcons_defsetf(local, instance, symbol);
	if (sup != Nil)
		push_varcons_defsetf(local, instance, sup);
	list_heap(&one, symbol, init, sup, NULL);
	cons_heap(&opt, one, opt);
	nextcons_finish(one, cons);
	goto optional_loop;

rest_environment:
	environment_expander_defsetf(local, instance, env, one, cons);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_REST, one)) goto rest_argument;
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	fmte("Invalid ~A argument.", one, NULL);

rest_argument:
	GetCdr(cons, &cons);
	if (cons == Nil)
		fmte("&rest parameter must be a one argument.", NULL);
	if (! consp(cons))
		fmte("Dot list don't allow in this lambda-list.", NULL);
	GetCar(cons, &rest);
	push_varcons_defsetf(local, instance, rest);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto key_environment;
	fmte("After &rest/&body ~S must be a null or &key arguments.", one, NULL);

key_environment:
	environment_expander_defsetf(local, instance, env, one, cons);
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_KEY, one)) goto key_argument;
	fmte("Invalid ~A argument.", one, NULL);

key_argument:
	key_p = T;
	nextcons_finish(one, cons);
key_loop:
	if (constant_eq(CONSTANT_AMPERSAND_ALLOW, one)) goto allow_argument;
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto finish_environment;
	ordinary_key(local, name, one, &symbol, &one, &init, &sup);
	push_varcons_defsetf(local, instance, symbol);
	if (sup != Nil)
		push_varcons_defsetf(local, instance, sup);
	list_heap(&one, symbol, one, init, sup, NULL);
	cons_heap(&key, one, key);
	nextcons_finish(one, cons);
	goto key_loop;

allow_argument:
	allow = T;
	nextcons_finish(one, cons);
	if (constant_eq(CONSTANT_AMPERSAND_ENVIRONMENT, one)) goto finish_environment;
	fmte("Invalid ~A argument.", one, NULL);

finish_environment:
	environment_expander_defsetf(local, instance, env, one, cons);
	nextcons_finish(one, cons);
	fmte("Invalid ~A argument.", one, NULL);

finish:
	list_heap(ret,
			nreverse_list_unsafe_inplace(var),
			nreverse_list_unsafe_inplace(opt),
			rest,
			key != Nil? nreverse_list_unsafe_inplace(key): key_p,
			allow,
			env,
			NULL);
}


/*
 *  control
 */
_g void getenvironment_macro_lambda(addr pos, addr *ret)
{
	addr var, opt, rest, key, allow, aux, whole;
	List_bind(pos, &var, &opt, &rest, &key, &allow, &aux, &whole, ret, NULL);
}

static void varsymbol_macro_lambda_heap(addr *ret, addr args)
{
	addr root, pos, cons;
	addr var, opt, rest, key, allow, aux, whole, env;

	List_bind(args, &var, &opt, &rest, &key, &allow, &aux, &whole, &env, NULL);
	root = *ret;

	/* var */
	while (var != Nil) {
		GetCons(var, &pos, &var);
		if (consp(pos))
			varsymbol_macro_lambda_heap(&root, pos);
		else
			cons_heap(&root, pos, root);
	}
	/* opt */
	while (opt != Nil) {
		GetCons(opt, &cons, &opt);
		GetCons(cons, &pos, &cons); /* var */
		cons_heap(&root, pos, root);
		GetCdr(cons, &cons); /* init */
		GetCar(cons, &pos); /* svar */
		if (pos != Nil)
			cons_heap(&root, pos, root);
	}
	/* rest */
	if (rest != Nil) {
		GetCar(rest, &pos);
		cons_heap(&root, pos, root);
	}
	/* key */
	while (key != Nil) {
		GetCons(key, &cons, &key);
		GetCons(cons, &pos, &cons); /* var */
		cons_heap(&root, pos, root);
		GetCdr(cons, &cons); /* name */
		GetCdr(cons, &cons); /* init */
		GetCar(cons, &pos); /* svar */
		if (pos != Nil)
			cons_heap(&root, pos, root);
	}
	/* aux */
	while (opt != Nil) {
		GetCons(opt, &cons, &opt);
		GetCar(cons, &pos); /* var */
		cons_heap(&root, pos, root);
	}
	/* whole */
	if (whole != Nil)
		cons_heap(&root, whole, root);
	/* environment */
	if (env != Nil)
		cons_heap(&root, env, root);
	/* result */
	*ret = root;
}

_g void allsymbol_macro_lambda_heap(LocalRoot local, addr *ret, addr args)
{
	addr root;

	lambda_macro(local, &args, args, Nil);
	root = Nil;
	varsymbol_macro_lambda_heap(&root, args);
	nreverse_list_unsafe(ret, root);
}


/*
 *  argument
 */
_g int argumentp(addr pos)
{
	return GetType(pos) == LISPSYSTEM_ARGUMENT;
}

_g void getargument(addr pos, size_t index, addr *ret)
{
	CheckType(pos, LISPSYSTEM_ARGUMENT);
	GetArgument_Low(pos, index, ret);
}

_g void setargument(addr pos, size_t index, addr value)
{
	CheckType(pos, LISPSYSTEM_ARGUMENT);
	SetArgument_Low(pos, index, value);
}

_g struct argument_struct *argumentstruct(addr pos)
{
	CheckType(pos, LISPSYSTEM_ARGUMENT);
	return ArgumentStruct_Low(pos);
}

_g void argument_alloc(LocalRoot local, addr *ret)
{
	addr pos;
	struct argument_struct *str;

	alloc_smallsize(local, &pos, LISPSYSTEM_ARGUMENT,
			ArgumentIndex_size, sizeoft(struct argument_struct));
	str = ArgumentStruct(pos);
	clearpoint(str);
	*ret = pos;
}

_g void argument_local(LocalRoot local, addr *ret)
{
	CheckLocal(local);
	argument_alloc(local, ret);
}

_g void argument_heap(addr *ret)
{
	argument_alloc(NULL, ret);
}

static void argument_ordinary_heap_g(LocalRoot local, addr *ret, addr list, addr g)
{
	addr pos, var, opt, rest, key, allow, aux;
	struct argument_struct *str;

	CheckLocal(local);
	/* parse */
	lambda_ordinary_g(local, &list, list, g);
	List_bind(list, &var, &opt, &rest, &key, &allow, &aux, NULL);
	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_ordinary;
	/* var */
	str->var = length_list_unsafe(var);
	SetArgument(pos, ArgumentIndex_var, var);
	/* opt */
	str->opt = length_list_unsafe(opt);
	SetArgument(pos, ArgumentIndex_opt, opt);
	/* rest */
	if (rest != Nil) {
		str->rest = 1;
		str->restbody = 1;
		SetArgument(pos, ArgumentIndex_rest, rest);
		SetArgument(pos, ArgumentIndex_restbody, rest);
	}
	/* key */
	if (key != Nil) {
		str->keyp = 1;
		str->key = length_list_unsafe(key);
		SetArgument(pos, ArgumentIndex_key, key);
	}
	/* allow-other-keys */
	if (allow != Nil) {
		str->allow = 1;
	}
	/* aux */
	str->aux = length_list_unsafe(aux);
	SetArgument(pos, ArgumentIndex_aux, aux);
	/* result */
	*ret = pos;
}

_g void argument_ordinary_heap(LocalRoot local, addr *ret, addr list)
{
	argument_ordinary_heap_g(local, ret, list, Nil);
}

_g void argument_generic_heap(LocalRoot local, addr *ret, addr list)
{
	addr pos, var, opt, rest, key, allow;
	struct argument_struct *str;

	CheckLocal(local);
	/* parse */
	lambda_generic_function(local, &list, list);
	List_bind(list, &var, &opt, &rest, &key, &allow, NULL);
	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_generic;
	/* var */
	str->var = length_list_unsafe(var);
	SetArgument(pos, ArgumentIndex_var, var);
	/* opt */
	str->opt = length_list_unsafe(opt);
	SetArgument(pos, ArgumentIndex_opt, opt);
	/* rest */
	if (rest != Nil) {
		str->rest = 1;
		str->restbody = 1;
		SetArgument(pos, ArgumentIndex_rest, rest);
		SetArgument(pos, ArgumentIndex_restbody, rest);
	}
	/* key */
	if (key == T) {
		str->keyp = 1;
	}
	else if (key != Nil) {
		str->keyp = 1;
		str->key = length_list_unsafe(key);
		SetArgument(pos, ArgumentIndex_key, key);
	}
	/* allow-other-keys */
	if (allow != Nil) {
		str->allow = 1;
	}
	/* result */
	*ret = pos;
}

_g void argument_method_heap(LocalRoot local, addr *ret, addr list)
{
	addr pos, var, opt, rest, key, allow, aux;
	struct argument_struct *str;

	CheckLocal(local);
	/* parse */
	lambda_specialized(local, &list, list);
	List_bind(list, &var, &opt, &rest, &key, &allow, &aux, NULL);
	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = length_list_unsafe(var);
	SetArgument(pos, ArgumentIndex_var, var);
	/* opt */
	str->opt = length_list_unsafe(opt);
	SetArgument(pos, ArgumentIndex_opt, opt);
	/* rest */
	if (rest != Nil) {
		str->rest = 1;
		str->restbody = 1;
		SetArgument(pos, ArgumentIndex_rest, rest);
		SetArgument(pos, ArgumentIndex_restbody, rest);
	}
	/* key */
	if (key == T) {
		str->keyp = 1;
	}
	else if (key != Nil) {
		str->keyp = 1;
		str->key = length_list_unsafe(key);
		SetArgument(pos, ArgumentIndex_key, key);
	}
	/* allow-other-keys */
	if (allow != Nil) {
		str->allow = 1;
	}
	/* aux */
	str->aux = length_list_unsafe(aux);
	SetArgument(pos, ArgumentIndex_aux, aux);
	/* result */
	*ret = pos;
}

_g void argument_combination_heap(LocalRoot local, addr *ret, addr list)
{
	addr whole, a, b, check;
	struct argument_struct *str;

	whole = Nil;
	if (! consp(list))
		goto ordinary;
	GetCons(list, &a, &b);
	GetConst(AMPERSAND_WHOLE, &check);
	if (a != check)
		goto ordinary;
	if (! consp(b))
		fmte("There is no variable after &whole.", NULL);
	GetCons(b, &whole, &list);
	variable_check(whole, AMPERSAND_ORDINARY);

ordinary:
	argument_ordinary_heap(local, &list, list);
	str = ArgumentStruct(list);
	str->type = ArgumentType_combination;
	if (whole != Nil) {
		str->whole = 1;
		SetArgument(list, ArgumentIndex_whole, whole);
	}
	*ret = list;
}

_g void argument_boa_heap(LocalRoot local, addr *ret, addr list, addr g)
{
	struct argument_struct *str;

	argument_ordinary_heap_g(local, &list, list, g);
	str = ArgumentStruct(list);
	str->type = ArgumentType_boa;
	*ret = list;
}


/*
 *  expand
 */
_g void argument_generic_lambda_heap(addr *ret, addr pos)
{
	addr root, list, var, a, b;
	struct argument_struct *str;

	root = Nil;
	str = ArgumentStruct(pos);
	Check(str->type != ArgumentType_generic, "type error");
	/* var */
	if (str->var) {
		GetArgument(pos, ArgumentIndex_var, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			cons_heap(&root, var, root);
		}
	}

	/* opt */
	if (str->opt) {
		GetConst(AMPERSAND_OPTIONAL, &list);
		cons_heap(&root, list, root);
		GetArgument(pos, ArgumentIndex_opt, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			cons_heap(&root, var, root);
		}
	}

	/* rest */
	if (str->rest) {
		GetConst(AMPERSAND_REST, &list);
		cons_heap(&root, list, root);
		GetArgument(pos, ArgumentIndex_rest, &list);
		cons_heap(&root, list, root);
	}

	/* key */
	if (str->keyp || str->key) {
		GetConst(AMPERSAND_KEY, &list);
		cons_heap(&root, list, root);
	}
	if (str->key) {
		GetArgument(pos, ArgumentIndex_key, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			list_bind(var, &a, &b, NULL);
			list_heap(&var, b, a, NULL);
			cons_heap(&root, var, root);
		}
	}

	/* allow-other-keys */
	if (str->allow) {
		GetConst(AMPERSAND_ALLOW, &list);
		cons_heap(&root, list, root);
	}

	/* result */
	nreverse_list_unsafe(ret, root);
}

static void argument_expand_heap(addr *ret, addr pos)
{
	addr root, list, var, a, b, c, d;
	struct argument_struct *str;

	root = Nil;
	str = ArgumentStruct(pos);
	/* var */
	if (str->var) {
		GetArgument(pos, ArgumentIndex_var, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			if (str->type == ArgumentType_method) {
				GetCar(var, &var);
			}
			cons_heap(&root, var, root);
		}
	}

	/* opt */
	if (str->opt) {
		GetConst(AMPERSAND_OPTIONAL, &list);
		cons_heap(&root, list, root);
		GetArgument(pos, ArgumentIndex_opt, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			list_bind(var, &a, &b, &c, NULL);
			if (c == Nil)
				list_heap(&var, a, b, NULL);
			else
				list_heap(&var, a, b, c, NULL);
			cons_heap(&root, var, root);
		}
	}

	/* rest */
	if (str->rest) {
		GetConst(AMPERSAND_REST, &list);
		cons_heap(&root, list, root);
		GetArgument(pos, ArgumentIndex_rest, &list);
		cons_heap(&root, list, root);
	}

	/* key */
	if (str->keyp || str->key) {
		GetConst(AMPERSAND_KEY, &list);
		cons_heap(&root, list, root);
	}
	if (str->key) {
		GetArgument(pos, ArgumentIndex_key, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			list_bind(var, &a, &b, &c, &d, NULL);
			list_heap(&a, b, a, NULL);
			if (d == Nil)
				list_heap(&var, a, c, NULL);
			else
				list_heap(&var, a, c, d, NULL);
			cons_heap(&root, var, root);
		}
	}

	/* allow-other-keys */
	if (str->allow) {
		GetConst(AMPERSAND_ALLOW, &list);
		cons_heap(&root, list, root);
	}

	/* aux */
	if (str->aux) {
		GetConst(AMPERSAND_AUX, &list);
		cons_heap(&root, list, root);
		GetArgument(pos, ArgumentIndex_aux, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			cons_heap(&root, var, root);
		}
	}

	/* result */
	nreverse_list_unsafe(ret, root);
}

_g void argument_ordinary_lambda_heap(addr *ret, addr pos)
{
	Check(ArgumentStruct(pos)->type != ArgumentType_ordinary, "type error");
	argument_expand_heap(ret, pos);
}

_g void argument_method_lambda_heap(addr *ret, addr pos)
{
	Check(ArgumentStruct(pos)->type != ArgumentType_method, "type error");
	argument_expand_heap(ret, pos);
}

_g void argument_method_keywords_heap(addr pos, addr *ret, int *allow)
{
	addr root, list, key;
	struct argument_struct *str;

	str = ArgumentStruct(pos);
	Check(str->type != ArgumentType_method, "type error");
	/* key */
	GetArgument(pos, ArgumentIndex_key, &list);
	for (root = Nil; list != Nil; ) {
		/* (var name init sup) */
		getcons(list, &key, &list);
		getcdr(key, &key);
		getcar(key, &key);
		cons_heap(&root, key, root);
	}
	/* result */
	nreverse_list_unsafe(ret, root);
	*allow = (int)str->allow;
}

_g void argument_method_to_generic(addr method, addr *ret)
{
	addr pos, list, root, var;
	struct argument_struct *str1, *str2;

	/* method */
	str1 = ArgumentStruct(method);
	Check(str1->type != ArgumentType_method, "type error");

	/* generic */
	argument_heap(&pos);
	str2 = ArgumentStruct(pos);
	str2->type = ArgumentType_generic;
	/* var */
	str2->var = str1->var;
	GetArgument(method, ArgumentIndex_var, &list);
	for (root = Nil; list != Nil; ) {
		GetCons(list, &var, &list);
		cons_heap(&root, var, root);
	}
	SetArgument(pos, ArgumentIndex_var, root);
	/* opt */
	str2->opt = str1->opt;
	GetArgument(method, ArgumentIndex_opt, &list);
	for (root = Nil; list != Nil; ) {
		GetCons(list, &var, &list);
		cons_heap(&root, var, root);
	}
	SetArgument(pos, ArgumentIndex_opt, root);
	/* rest */
	str2->rest = str1->rest;
	str2->restbody = str1->restbody;
	GetArgument(method, ArgumentIndex_rest, &list);
	SetArgument(pos, ArgumentIndex_rest, list);
	/* key */
	str2->key = str1->key;
	str2->keyp = str1->keyp;
	GetArgument(method, ArgumentIndex_key, &list);
	for (root = Nil; list != Nil; ) {
		GetCons(list, &var, &list);
		cons_heap(&root, var, root);
	}
	SetArgument(pos, ArgumentIndex_key, root);
	/* aux */
	str2->aux = str1->aux;
	GetArgument(method, ArgumentIndex_aux, &list);
	for (root = Nil; list != Nil; ) {
		GetCons(list, &var, &list);
		cons_heap(&root, var, root);
	}
	SetArgument(pos, ArgumentIndex_aux, root);
	/* result */
	*ret = pos;
}

_g void argument_boa_lambda_heap(addr *ret, addr pos)
{
	Check(ArgumentStruct(pos)->type != ArgumentType_boa, "type error");
	argument_expand_heap(ret, pos);
}

_g void argument_boa_variables_heap(addr *ret, addr pos)
{
	addr root, list, var, a, b, c, d;
	struct argument_struct *str;

	root = Nil;
	str = ArgumentStruct(pos);
	/* var */
	if (str->var) {
		GetArgument(pos, ArgumentIndex_var, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			if (str->type == ArgumentType_method) {
				GetCar(var, &var);
			}
			cons_heap(&root, var, root);
		}
	}

	/* opt */
	if (str->opt) {
		GetArgument(pos, ArgumentIndex_opt, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			list_bind(var, &a, &b, &c, NULL);
			cons_heap(&root, a, root);
		}
	}

	/* rest */
	if (str->rest) {
		GetArgument(pos, ArgumentIndex_rest, &a);
		cons_heap(&root, a, root);
	}

	/* key */
	if (str->key) {
		GetArgument(pos, ArgumentIndex_key, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			list_bind(var, &a, &b, &c, &d, NULL);
			cons_heap(&root, a, root);
		}
	}

	/* aux */
	if (str->aux) {
		GetArgument(pos, ArgumentIndex_aux, &list);
		while (list != Nil) {
			GetCons(list, &var, &list);
			GetCar(var, &a);
			cons_heap(&root, a, root);
		}
	}

	/* result */
	nreverse_list_unsafe(ret, root);
}

