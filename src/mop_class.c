/*
 *  ANSI COMMON LISP: 7. Objects
 *    Common Lisp Object System - Metaobject Protocol
 */
#include "clos.h"
#include "clos_class.h"
#include "clos_common.h"
#include "clos_generic.h"
#include "clos_method.h"
#include "clos_type.h"
#include "condition.h"
#include "cons.h"
#include "constant.h"
#include "control.h"
#include "execute.h"
#include "function.h"
#include "lambda.h"
#include "mop.h"
#include "structure.h"
#include "symbol.h"
#include "type_table.h"

/***********************************************************************
 *  referenced-class
 ***********************************************************************/
/* (defun system::referenced-class (symbol) ...) -> class */
static void function_referenced_class(Execute ptr, addr symbol)
{
	addr pos;

	/* find-class */
	clos_find_class_nil(symbol, &pos);
	if (pos != Nil) {
		setresult_control(ptr, pos);
		return;
	}

	/* forward-referenced-class */
	GetConst(CLOS_FORWARD_REFERENCED_CLASS, &pos);
	clos_instance_heap(pos, &pos);
	stdset_class_name(pos, symbol);
	setresult_control(ptr, pos);
}

static void type_referenced_class(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Symbol);
	typeargs_var1(&args, args);
	GetTypeValues(&values, Class);
	type_compiled_heap(args, values, ret);
}

static void defun_referenced_class_mop(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(CLOSNAME_REFERENCED_CLASS, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, p_defun_referenced_class);
	SetFunctionSymbol(symbol, pos);
	/* type */
	type_referenced_class(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/***********************************************************************
 *  ensure-class
 ***********************************************************************/
/* (defun system::ensure-class
 *     (name &rest args &key &allow-other-keys) ...)
 *     -> class
 *   name   symbol
 *   args   t
 *   class  class
 */
static void function_ensure_class(Execute ptr, addr name, addr rest)
{
	addr symbol, clos, check;
	LocalRoot local;
	LocalStack stack;

	/* class check */
	clos_find_class_nil(name, &clos);
	if (clos != Nil) {
		GetConst(COMMON_CLASS_NAME, &symbol);
		if (callclang_funcall(ptr, &check, symbol, clos, NULL))
			return;
		if (check != name)
			clos = Nil;
	}

	/* call */
	GetConst(CLOSNAME_ENSURE_CLASS_USING_CLASS, &symbol);
	getfunctioncheck_local(ptr, symbol, &symbol);
	local = ptr->local;
	push_local(local, &stack);
	lista_local(local, &rest, clos, name, rest, NULL);
	if (apply_control(ptr, symbol, rest)) return;
	rollback_local(local, stack);
}

static void type_ensure_class(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Symbol);
	GetTypeTable(&values, T);
	typeargs_var1rest(&args, args, values);
	GetTypeValues(&values, Class);
	type_compiled_heap(args, values, ret);
}

static void defun_ensure_class_mop(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(CLOSNAME_ENSURE_CLASS, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1dynamic(pos, p_defun_ensure_class);
	SetFunctionSymbol(symbol, pos);
	/* type */
	type_ensure_class(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/***********************************************************************
 *  ensure-class-using-class
 ***********************************************************************/
/* (defmethod ensure-class-using-class
 *     ((inst null) name
 *      &key metaclass direct-superclasses &allow-other-keys) ...)
 *     -> class
 *   inst                 null
 *   name                 symbol
 *   metaclass            class
 *   direct-superclasses  list
 *   class                class
 */
static void method_ensure_class_using_class_null(Execute ptr,
		addr method, addr next, addr clos, addr name, addr rest)
{
	Check(clos != Nil, "type error");
	if (clos_ensure_class(ptr, name, rest, &clos)) return;
	setresult_control(ptr, clos);
}

static void method_type_ensure_class_using_class(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Symbol);
	typeargs_var2rest(&args, args, values, args);
	typeargs_method(args);
	GetTypeValues(&values, Class);
	type_compiled_heap(args, values, ret);
}

static void argument_method_ensure_class_using_class(addr *ret, constindex type)
{
	addr pos, key, key1, key2;
	struct argument_struct *str;

	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	str->var = 2;
	str->keyp = 1;
	str->key = 2;
	str->allow = 1;
	/* var */
	GetConstant(type, &key1);
	list_heap(&key1, Nil, key1, NULL);
	GetConst(CLOS_T, &key2);
	list_heap(&key2, Nil, key2, NULL);
	list_heap(&key, key1, key2, NULL);
	SetArgument(pos, ArgumentIndex_var, key);
	/* key */
	GetConst(CLOSKEY_METACLASS, &key1);
	conscar_heap(&key1, key1);
	GetConst(CLOSKEY_DIRECT_SUPERCLASSES, &key2);
	conscar_heap(&key2, key2);
	list_heap(&key, key1, key2, NULL);
	SetArgument(pos, ArgumentIndex_key, key);
	/* result */
	*ret = pos;
}

static void defmethod_ensure_class_using_class_null(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var4dynamic(call, p_method_ensure_class_using_class_null);
	method_type_ensure_class_using_class(&type);
	settype_function(call, type);
	/* method */
	argument_method_ensure_class_using_class(&pos, CONSTANT_CLOS_NULL);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defmethod ensure-class-using-class
 *     ((inst standard-class) name
 *      &key metaclass direct-superclasses direct-slots &allow-other-keys) ...)
 *     -> class
 *   inst                 standard-class
 *   name                 symbol
 *   metaclass            class
 *   direct-superclasses  list
 *   direct-slots         list
 *   class                class
 */
static void method_ensure_class_using_class_class(Execute ptr,
		addr method, addr next, addr clos, addr name, addr rest)
{
	CheckType(clos, LISPTYPE_CLOS);
	if (clos_ensure_class_redefine(ptr, clos, name, rest)) return;
	setresult_control(ptr, clos);
}

static void defmethod_ensure_class_using_class_class(Execute ptr,
		addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var4dynamic(call, p_method_ensure_class_using_class_class);
	method_type_ensure_class_using_class(&type);
	settype_function(call, type);
	/* method */
	argument_method_ensure_class_using_class(&pos, CONSTANT_CLOS_CLASS);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defgeneric ensure-class-using-class
 *     (class name &rest initargs &key &allow-other-keys) ...)
 *   -> class
 */
static void defgeneric_ensure_class_using_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(CLOSNAME_ENSURE_CLASS_USING_CLASS, &symbol);
	mop_argument_generic_var2rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_ensure_class_using_class_null(ptr, name, gen);
	defmethod_ensure_class_using_class_class(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  allocate-instance
 ***********************************************************************/
/* (defmethod allocate-instance
 *     ((class standard-class) &rest args &key)) -> instance
 */
static void method_allocate_instance_stdclass(Execute ptr,
		addr method, addr next, addr clos, addr rest)
{
	allocate_instance_stdclass(ptr, clos, &clos);
	setresult_control(ptr, clos);
}

static void method_type_allocate_instance(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	typeargs_var1rest(&args, args, args);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void argument_method_allocate_instance(addr *ret, constindex type)
{
	addr pos, key;
	struct argument_struct *str;

	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	str->var = 1;
	str->keyp = 1;
	str->allow = 1;
	/* var */
	GetConstant(type, &key);
	list_heap(&key, Nil, key, NULL);
	list_heap(&key, key, NULL);
	SetArgument(pos, ArgumentIndex_var, key);
	/* result */
	*ret = pos;
}

static void defmethod_allocate_instance(Execute ptr,
		addr name, addr gen, constindex index)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var3dynamic(call, p_method_allocate_instance_stdclass);
	method_type_allocate_instance(&type);
	settype_function(call, type);
	/* method */
	argument_method_allocate_instance(&pos, index);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defgeneric allocate-instance (class &rest args &key)) -> instance */
static void defgeneric_allocate_instance_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_ALLOCATE_INSTANCE, &symbol);
	mop_argument_generic_var1rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_allocate_instance(ptr, name, gen, CONSTANT_CLOS_STANDARD_CLASS);
	defmethod_allocate_instance(ptr, name, gen, CONSTANT_CLOS_STRUCTURE_CLASS);
	common_method_finalize(gen);
}


/***********************************************************************
 *  initialize-instance
 ***********************************************************************/
/* (defmethod initialize-instance
 *     ((object standard-object) &rest args &key)) -> instance
 */
static void method_initialize_instance_stdobject(Execute ptr,
		addr method, addr next, addr pos, addr rest)
{
	if (initialize_instance_stdobject(ptr, pos, rest, &pos)) return;
	setresult_control(ptr, pos);
}

static void defmethod_initialize_instance_stdobject(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var3dynamic(call, p_method_initialize_instance_stdobject);
	method_type_allocate_instance(&type);
	settype_function(call, type);
	/* method */
	argument_method_allocate_instance(&pos, CONSTANT_CLOS_STANDARD_OBJECT);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defgeneric initialize-instance (class &rest args &key)) -> instance */
static void defgeneric_initialize_instance_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_INITIALIZE_INSTANCE, &symbol);
	mop_argument_generic_var1rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_initialize_instance_stdobject(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  reinitialize-instance
 ***********************************************************************/
/* (defmethod reinitialize-instance
 *     ((object standard-object) &rest args &key)) -> instance
 */
static void method_reinitialize_instance_stdobject(Execute ptr,
		addr method, addr next, addr pos, addr rest)
{
	if (reinitialize_instance_stdobject(ptr, pos, rest, &pos)) return;
	setresult_control(ptr, pos);
}

static void defmethod_reinitialize_instance_stdobject(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var3dynamic(call, p_method_reinitialize_instance_stdobject);
	method_type_allocate_instance(&type);
	settype_function(call, type);
	/* method */
	argument_method_allocate_instance(&pos, CONSTANT_CLOS_STANDARD_OBJECT);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defgeneric reinitialize-instance (class &rest args &key)) -> instance */
static void defgeneric_reinitialize_instance_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_REINITIALIZE_INSTANCE, &symbol);
	mop_argument_generic_var1rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_reinitialize_instance_stdobject(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  shared-initialize
 ***********************************************************************/
/* (defmethod shared-initialize
 *     ((object standard-object) name &rest args &key)) -> instance
 */
static void method_shared_initialize_stdobject(Execute ptr,
		addr method, addr next, addr pos, addr name, addr rest)
{
	if (shared_initialize_stdobject(ptr, pos, name, rest)) return;
	setresult_control(ptr, pos);
}

static void method_type_shared_initialize(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	typeargs_var2rest(&args, args, args, args);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void argument_method_shared_initialize(addr *ret, constindex type)
{
	addr pos, key, key1, key2;
	struct argument_struct *str;

	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	str->var = 2;
	str->keyp = 1;
	str->allow = 1;
	/* var */
	GetConstant(type, &key1);
	list_heap(&key1, Nil, key1, NULL);
	GetConst(CLOS_T, &key2);
	list_heap(&key2, Nil, key2, NULL);
	list_heap(&key, key1, key2, NULL);
	SetArgument(pos, ArgumentIndex_var, key);
	/* result */
	*ret = pos;
}

static void defmethod_shared_initialize_stdobject(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var4dynamic(call, p_method_shared_initialize_stdobject);
	method_type_shared_initialize(&type);
	settype_function(call, type);
	/* method */
	argument_method_shared_initialize(&pos, CONSTANT_CLOS_STANDARD_OBJECT);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defgeneric shared-initialize (class &rest args &key)) -> instance */
static void defgeneric_shared_initialize_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_SHARED_INITIALIZE, &symbol);
	mop_argument_generic_var2rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_shared_initialize_stdobject(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  make-instance
 ***********************************************************************/
/* (defmethod make-instance
 *     ((class symbol) &rest initargs &key) ...)
 *   -> instance
 */
static void method_make_instance_symbol(Execute ptr,
		addr method, addr next, addr var, addr rest)
{
	addr symbol;
	LocalRoot local;
	LocalStack stack;

	Check(! symbolp(var), "type error");
	clos_find_class(var, &var);
	/* call generic-function */
	GetConst(COMMON_MAKE_INSTANCE, &symbol);
	getfunctioncheck_local(ptr, symbol, &symbol);
	local = ptr->local;
	push_local(local, &stack);
	lista_local(local, &rest, var, rest, NULL);
	if (apply_control(ptr, symbol, rest)) return;
	rollback_local(local, stack);
}

static void type_make_instance_symbol(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, Symbol);
	GetTypeTable(&values, T);
	typeargs_var1rest(&args, args, values);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defmethod_make_instance_symbol(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var3dynamic(call, p_method_make_instance_symbol);
	type_make_instance_symbol(&type);
	settype_function(call, type);
	/* method */
	ArgumentMethod_var1rest(&pos, SYMBOL);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defmethod make-instance
 *     ((class standard-class) &rest initargs &key) ...)
 *   -> instance
 */
static void method_make_instance_stdclass(Execute ptr,
		addr method, addr next, addr rest)
{
	if (make_instance_stdclass(ptr, rest, &rest))
		return;
	setresult_control(ptr, rest);
}

static void method_type_make_instance_stdclass(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, StandardClass);
	GetTypeTable(&values, T);
	typeargs_var1rest(&args, args, values);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defmethod_make_instance_stdclass(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var2dynamic(call, p_method_make_instance_stdclass);
	method_type_make_instance_stdclass(&type);
	settype_function(call, type);
	/* method */
	ArgumentMethod_var1rest(&pos, STANDARD_CLASS);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defmethod make-instance
 *     ((class structure-class) &rest initargs &key) ...)
 *   -> instance
 */
static void method_make_instance_structure(Execute ptr,
		addr method, addr next, addr rest)
{
	if (make_instance_structure(ptr, rest, &rest))
		return;
	setresult_control(ptr, rest);
}

static void method_type_make_instance_structure(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, StructureClass);
	GetTypeTable(&values, T);
	typeargs_var1rest(&args, args, values);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void defmethod_make_instance_structure(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var2dynamic(call, p_method_make_instance_structure);
	method_type_make_instance_structure(&type);
	settype_function(call, type);
	/* method */
	ArgumentMethod_var1rest(&pos, STRUCTURE_CLASS);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}


/* (defgeneric make-instance
 *      (class &rest initargs &key allow-other-keys) ...)
 *    -> instance
 */
static void defgeneric_make_instance_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_MAKE_INSTANCE, &symbol);
	mop_argument_generic_var1rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_make_instance_symbol(ptr, name, gen);
	defmethod_make_instance_stdclass(ptr, name, gen);
	defmethod_make_instance_structure(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  slot-missing
 ***********************************************************************/
static void method_slot_missing(Execute ptr,
		addr method, addr next, addr rest)
{
	addr c, obj, name, op, value;

	lista_bind(rest, &c, &obj, &name, &op, &value, NULL);
	fmte("The class ~S has no slot ~S name ~S operation.", c, name, op, NULL);
}

static void method_type_slot_missing(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Symbol);
	typeargs_var4opt1(&args, args, args, values, values, args);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void method_argument_slot_missing(addr *ret)
{
	addr pos, list, type1, type2;
	struct argument_struct *str;

	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = 4;
	str->opt = 1;
	ArgumentMethod_var(&type1, T);
	ArgumentMethod_var(&type2, SYMBOL);
	list_heap(&list, type1, type1, type2, type2, NULL);
	SetArgument(pos, ArgumentIndex_var, list);
	/* result */
	*ret = pos;
}

static void defmethod_slot_missing(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var2dynamic(call, p_method_slot_missing);
	method_type_slot_missing(&type);
	settype_function(call, type);
	/* method */
	method_argument_slot_missing(&pos);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void mop_argument_generic_var4opt1(addr *ret)
{
	addr pos;
	struct argument_struct *str;

	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_generic;
	str->var = 4;
	str->opt = 1;
	*ret = pos;
}

static void defgeneric_slot_missing_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_SLOT_MISSING, &symbol);
	mop_argument_generic_var4opt1(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_slot_missing(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  slot-unbound
 ***********************************************************************/
static void method_slot_unbound(Execute ptr,
		addr method, addr next, addr rest)
{
	addr clos, obj, name;

	list_bind(rest, &clos, &obj, &name, NULL);
	fmte("The slot ~S is unbound in the ~S.", name, obj, NULL);
}

static void method_type_slot_unbound(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Symbol);
	typeargs_var3(&args, args, args, values);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void method_argument_slot_unbound(addr *ret)
{
	addr pos, list, type1, type2;
	struct argument_struct *str;

	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = 3;
	ArgumentMethod_var(&type1, T);
	ArgumentMethod_var(&type2, SYMBOL);
	list_heap(&list, type1, type1, type2, NULL);
	SetArgument(pos, ArgumentIndex_var, list);
	/* result */
	*ret = pos;
}

static void defmethod_slot_unbound(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var2dynamic(call, p_method_slot_unbound);
	method_type_slot_unbound(&type);
	settype_function(call, type);
	/* method */
	method_argument_slot_unbound(&pos);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_slot_unbound_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_SLOT_UNBOUND, &symbol);
	mop_argument_generic_var3(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_slot_unbound(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  update-instance-for-different-class
 ***********************************************************************/
static void defgeneric_update_instance_for_different_class_mop()
{
	addr symbol, name, gen;

	GetConst(COMMON_UPDATE_INSTANCE_FOR_DIFFERENT_CLASS, &symbol);
	mop_argument_generic_var2rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* no-method */
	common_method_finalize(gen);
}


/***********************************************************************
 *  update-instance-for-redefined-class
 ***********************************************************************/
static void defgeneric_update_instance_for_redefined_class_mop()
{
	addr symbol, name, gen;

	GetConst(COMMON_UPDATE_INSTANCE_FOR_REDEFINED_CLASS, &symbol);
	mop_argument_generic_var4rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* no-method */
	common_method_finalize(gen);
}


/***********************************************************************
 *  slot-boundp-using-class
 ***********************************************************************/
static void method_slot_boundp_using_class(Execute ptr,
		addr method, addr next, addr clos, addr pos, addr name)
{
	int check;
	if (slot_boundp_using_class_common(ptr, clos, pos, name, &check)) return;
	setbool_control(ptr, check);
}

static void method_argument_slot_boundp_using_class(addr *ret, constindex index)
{
	addr pos, list, type1, type2, type3;
	struct argument_struct *str;

	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = 3;
	mop_argument_method_var(&type1, index);
	ArgumentMethod_var(&type2, T);
	ArgumentMethod_var(&type3, SYMBOL);
	list_heap(&list, type1, type2, type3, NULL);
	SetArgument(pos, ArgumentIndex_var, list);
	/* result */
	*ret = pos;
}

static void defmethod_slot_boundp_using_class(Execute ptr,
		addr name, addr gen, constindex index)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var5(call, p_method_slot_boundp_using_class);
	GetTypeCompiled(&type, SlotBoundp_Method);
	settype_function(call, type);
	/* method */
	method_argument_slot_boundp_using_class(&pos, index);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_slot_boundp_using_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(CLOSNAME_SLOT_BOUNDP_USING_CLASS, &symbol);
	mop_argument_generic_var3(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_slot_boundp_using_class(ptr, name, gen, CONSTANT_CLOS_STANDARD_CLASS);
	defmethod_slot_boundp_using_class(ptr, name, gen, CONSTANT_CLOS_STRUCTURE_CLASS);
	common_method_finalize(gen);
}


/***********************************************************************
 *  slot-exists-p-using-class
 ***********************************************************************/
static void method_slot_exists_p_using_class(Execute ptr,
		addr method, addr next, addr clos, addr pos, addr name)
{
	setbool_control(ptr, clos_slot_exists_p(pos, name));
}

static void defmethod_slot_exists_p_using_class(Execute ptr,
		addr name, addr gen, constindex index)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var5(call, p_method_slot_exists_p_using_class);
	GetTypeCompiled(&type, SlotBoundp_Method);
	settype_function(call, type);
	/* method */
	method_argument_slot_boundp_using_class(&pos, index);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_slot_exists_p_using_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(CLOSNAME_SLOT_EXISTS_P_USING_CLASS, &symbol);
	mop_argument_generic_var3(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_slot_exists_p_using_class(ptr, name, gen, CONSTANT_CLOS_STANDARD_CLASS);
	defmethod_slot_exists_p_using_class(ptr, name, gen, CONSTANT_CLOS_STRUCTURE_CLASS);
	common_method_finalize(gen);
}


/***********************************************************************
 *  slot-makunbound-using-class
 ***********************************************************************/
static void method_slot_makunbound_using_class(Execute ptr,
		addr method, addr next, addr clos, addr pos, addr name)
{
	if (slot_makunbound_using_class(ptr, clos, pos, name)) return;
	setresult_control(ptr, pos);
}

static void defmethod_slot_makunbound_using_class(Execute ptr,
		addr name, addr gen, constindex index)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var5(call, p_method_slot_makunbound_using_class);
	GetTypeCompiled(&type, SlotBoundp_Method);
	settype_function(call, type);
	/* method */
	method_argument_slot_boundp_using_class(&pos, index);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_slot_makunbound_using_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(CLOSNAME_SLOT_MAKUNBOUND_USING_CLASS, &symbol);
	mop_argument_generic_var3(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_slot_makunbound_using_class(ptr, name, gen, CONSTANT_CLOS_STANDARD_CLASS);
	defmethod_slot_makunbound_using_class(ptr, name, gen, CONSTANT_CLOS_STRUCTURE_CLASS);
	common_method_finalize(gen);
}


/***********************************************************************
 *  slot-value-using-class
 ***********************************************************************/
static void method_slot_value_using_class(Execute ptr,
		addr method, addr next, addr clos, addr pos, addr name)
{
	if (slot_value_using_class_common(ptr, clos, pos, name, &pos)) return;
	setresult_control(ptr, pos);
}

static void defmethod_slot_value_using_class(Execute ptr,
		addr name, addr gen, constindex index)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var5(call, p_method_slot_value_using_class);
	GetTypeCompiled(&type, SlotBoundp_Method);
	settype_function(call, type);
	/* method */
	method_argument_slot_boundp_using_class(&pos, index);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_slot_value_using_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(CLOSNAME_SLOT_VALUE_USING_CLASS, &symbol);
	mop_argument_generic_var3(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_slot_value_using_class(ptr, name, gen, CONSTANT_CLOS_STANDARD_CLASS);
	defmethod_slot_value_using_class(ptr, name, gen, CONSTANT_CLOS_STRUCTURE_CLASS);
	common_method_finalize(gen);
}


/***********************************************************************
 *  (setf slot-value-using-class)
 ***********************************************************************/
static void method_setf_slot_value_using_class(Execute ptr,
		addr method, addr next, addr rest)
{
	addr value, clos, pos, name;

	list_bind(rest, &value, &clos, &pos, &name, NULL);
	if (setf_slot_value_using_class_common(ptr, clos, pos, name, value)) return;
	setresult_control(ptr, value);
}

static void method_type_setf_slot_value_using_class(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Symbol);
	typeargs_var4(&args, args, args, args, values);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void method_argument_setf_slot_value_using_class(addr *ret)
{
	addr pos, list, type1, type2;
	struct argument_struct *str;

	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = 4;
	ArgumentMethod_var(&type1, T);
	ArgumentMethod_var(&type2, SYMBOL);
	list_heap(&list, type1, type1, type1, type2, NULL);
	SetArgument(pos, ArgumentIndex_var, list);
	/* result */
	*ret = pos;
}

static void defmethod_setf_slot_value_using_class(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var2dynamic(call, p_method_setf_slot_value_using_class);
	method_type_setf_slot_value_using_class(&type);
	settype_function(call, type);
	/* method */
	method_argument_setf_slot_value_using_class(&pos);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_setf_slot_value_using_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(CLOSNAME_SLOT_VALUE_USING_CLASS, &symbol);
	mop_argument_generic_var4(&gen);
	setf_callname_heap(&name, symbol);
	generic_common_instance(&gen, name, gen);
	setsetf_symbol(symbol, gen);
	/* method */
	defmethod_setf_slot_value_using_class(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  change-class
 ***********************************************************************/
static void method_change_class_stdclass(Execute ptr,
		addr method, addr next, addr pos, addr clos, addr rest)
{
	if (clos_change_class(ptr, pos, clos, rest, &pos)) return;
	setresult_control(ptr, pos);
}

static void method_type_change_class_stdclass(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, StandardObject);
	GetTypeTable(&values, StandardClass);
	typeargs_var2rest(&args, args, values, args);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void method_argument_change_class_stdclass(addr *ret)
{
	addr pos, list, type1, type2;
	struct argument_struct *str;

	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = 2;
	str->rest = 1;
	str->keyp = 1;
	ArgumentMethod_var(&type1, STANDARD_OBJECT);
	ArgumentMethod_var(&type2, STANDARD_CLASS);
	list_heap(&list, type1, type2, NULL);
	SetArgument(pos, ArgumentIndex_var, list);
	/* result */
	*ret = pos;
}

static void defmethod_change_class_stdclass(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var4dynamic(call, p_method_change_class_stdclass);
	method_type_change_class_stdclass(&type);
	settype_function(call, type);
	/* method */
	method_argument_change_class_stdclass(&pos);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void method_change_class_symbol(Execute ptr,
		addr method, addr next, addr pos, addr clos, addr rest)
{
	addr call;
	LocalRoot local;
	LocalStack stack;

	GetConst(COMMON_CHANGE_CLASS, &call);
	getfunctioncheck_local(ptr, call, &call);
	local = ptr->local;
	push_local(local, &stack);
	clos_find_class(clos, &clos);
	lista_local(local, &rest, pos, clos, rest, NULL);
	if (apply_control(ptr, call, rest)) return;
	rollback_local(local, stack);
}

static void method_type_change_class_symbol(addr *ret)
{
	addr args, values;

	GetTypeTable(&args, T);
	GetTypeTable(&values, Symbol);
	typeargs_var2rest(&args, args, values, args);
	typeargs_method(args);
	GetTypeValues(&values, T);
	type_compiled_heap(args, values, ret);
}

static void method_argument_change_class_symbol(addr *ret)
{
	addr pos, list, type1, type2;
	struct argument_struct *str;

	/* object */
	argument_heap(&pos);
	str = ArgumentStruct(pos);
	str->type = ArgumentType_method;
	/* var */
	str->var = 2;
	str->rest = 1;
	str->keyp = 1;
	ArgumentMethod_var(&type1, T);
	ArgumentMethod_var(&type2, SYMBOL);
	list_heap(&list, type1, type2, NULL);
	SetArgument(pos, ArgumentIndex_var, list);
	/* result */
	*ret = pos;
}

static void defmethod_change_class_symbol(Execute ptr, addr name, addr gen)
{
	addr pos, call, type;

	/* function */
	compiled_heap(&call, name);
	setcompiled_var4dynamic(call, p_method_change_class_symbol);
	method_type_change_class_symbol(&type);
	settype_function(call, type);
	/* method */
	method_argument_change_class_symbol(&pos);
	method_instance_lambda(ptr->local, &pos, Nil, pos);
	stdset_method_function(pos, call);
	common_method_add(ptr, gen, pos);
}

static void defgeneric_change_class_mop(Execute ptr)
{
	addr symbol, name, gen;

	GetConst(COMMON_CHANGE_CLASS, &symbol);
	mop_argument_generic_var2rest1key0(&gen);
	parse_callname_error(&name, symbol);
	generic_common_instance(&gen, name, gen);
	SetFunctionSymbol(symbol, gen);
	/* method */
	defmethod_change_class_stdclass(ptr, name, gen);
	defmethod_change_class_symbol(ptr, name, gen);
	common_method_finalize(gen);
}


/***********************************************************************
 *  function
 ***********************************************************************/
_g void init_mop_class(void)
{
	SetPointerCall(defun, var1, referenced_class);
	SetPointerCall(defun, var1dynamic, ensure_class);
	SetPointerType(var4dynamic, method_ensure_class_using_class_null);
	SetPointerType(var4dynamic, method_ensure_class_using_class_class);
	SetPointerType(var3dynamic, method_allocate_instance_stdclass);
	SetPointerType(var3dynamic, method_initialize_instance_stdobject);
	SetPointerType(var3dynamic, method_reinitialize_instance_stdobject);
	SetPointerType(var4dynamic, method_shared_initialize_stdobject);
	SetPointerType(var3dynamic, method_make_instance_symbol);
	SetPointerType(var2dynamic, method_make_instance_stdclass);
	SetPointerType(var2dynamic, method_make_instance_structure);
	SetPointerType(var2dynamic, method_slot_missing);
	SetPointerType(var2dynamic, method_slot_unbound);
	SetPointerType(var5, method_slot_boundp_using_class);
	SetPointerType(var5, method_slot_exists_p_using_class);
	SetPointerType(var5, method_slot_makunbound_using_class);
	SetPointerType(var5, method_slot_value_using_class);
	SetPointerType(var2dynamic, method_setf_slot_value_using_class);
	SetPointerType(var4dynamic, method_change_class_stdclass);
	SetPointerType(var4dynamic, method_change_class_symbol);
}

_g void build_mop_class(Execute ptr)
{
	defun_referenced_class_mop();
	defun_ensure_class_mop();
	defgeneric_ensure_class_using_class_mop(ptr);
	defgeneric_allocate_instance_mop(ptr);
	defgeneric_initialize_instance_mop(ptr);
	defgeneric_reinitialize_instance_mop(ptr);
	defgeneric_shared_initialize_mop(ptr);
	defgeneric_make_instance_mop(ptr);
	defgeneric_slot_missing_mop(ptr);
	defgeneric_slot_unbound_mop(ptr);
	defgeneric_update_instance_for_different_class_mop();
	defgeneric_update_instance_for_redefined_class_mop();
	defgeneric_slot_boundp_using_class_mop(ptr);
	defgeneric_slot_exists_p_using_class_mop(ptr);
	defgeneric_slot_makunbound_using_class_mop(ptr);
	defgeneric_slot_value_using_class_mop(ptr);
	defgeneric_setf_slot_value_using_class_mop(ptr);
	defgeneric_change_class_mop(ptr);
}

