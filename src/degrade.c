#include <stdio.h>
#include "define.h"
#include "degrade.h"

#ifdef LISP_DEGRADE
/*
 *  prototype declaration
 */
int test_c99(void);
int test_arch(void);
int test_alloc(void);
int test_memory(void);
int test_heap(void);
int test_local(void);
int test_execute(void);
int test_character(void);
int test_charqueue(void);
int test_unicode(void);
int test_array(void);
int test_array_object(void);
int test_strtype(void);
int test_object(void);
int test_symbol(void);
int test_function(void);
int test_cons(void);
int test_fasl(void);
int test_gc(void);
int test_sxhash(void);
int test_hashtable(void);
int test_sequence(void);
int test_sort(void);
int test_code(void);
int test_control(void);
int test_pathname(void);
int test_file_memory(void);
int test_encode(void);
int test_file(void);
int test_stream(void);
int test_integer(void);
int test_print(void);
int test_format(void);
int test_bigcons(void);
int test_bigdata(void);
int test_bignum(void);
int test_radix(void);
int test_ratio(void);
int test_number(void);
int test_token(void);
int test_readtable(void);
int test_quote(void);
int test_package(void);
int test_lambda(void);
int test_clos(void);
int test_clos_class(void);
int test_clos_type(void);
int test_clos_cache(void);
int test_clos_combination(void);
int test_clos_generic(void);
int test_clos_method(void);
int test_type(void);
int test_type_copy(void);
int test_type_name(void);
int test_type_number(void);
int test_type_object(void);
int test_type_optimize(void);
int test_type_parse(void);
int test_type_range(void);
int test_type_subtypep(void);
int test_type_symbol(void);
int test_type_table(void);
int test_type_typep(void);
int test_type_upgraded(void);
int test_type_value(void);
int test_eval(void);
int test_eval_declare(void);
int test_eval_parse(void);
int test_eval_optparse(void);
int test_eval_table(void);
int test_eval_stack(void);
int test_eval_scope(void);
int test_eval_code(void);
int test_equal(void);
int test_fmtfloat(void);
int test_condition(void);
int test_bit(void);
int test_main(void);
int loadrt(void);


/*
 *  degrade function
 */
void degrade_execute(void)
{
#if 0
#endif
	DegradeCheck(test_c99);
	DegradeCheck(test_arch);
	DegradeCheck(test_alloc);
	DegradeCheck(test_memory);
	DegradeCheck(test_local);
	DegradeCheck(test_heap);
	DegradeCheck(test_execute);
	DegradeCheck(test_object);
	DegradeCheck(test_symbol);
	DegradeCheck(test_function);
	DegradeCheck(test_cons);
	DegradeCheck(test_character);
	DegradeCheck(test_charqueue);
	DegradeCheck(test_unicode);
	DegradeCheck(test_array);
	DegradeCheck(test_array_object);
	DegradeCheck(test_strtype);
	DegradeCheck(test_gc);
	DegradeCheck(test_sxhash);
	DegradeCheck(test_hashtable);
	DegradeCheck(test_sequence);
	DegradeCheck(test_sort);
	DegradeCheck(test_code);
	DegradeCheck(test_control);
	DegradeCheck(test_pathname);
	DegradeCheck(test_file_memory);
	DegradeCheck(test_encode);
	DegradeCheck(test_file);
	//DegradeCheck(test_fasl); /* error */
	DegradeCheck(test_bigcons);
	DegradeCheck(test_bignum);
	DegradeCheck(test_radix);
	DegradeCheck(test_ratio);
	DegradeCheck(test_number);
	DegradeCheck(test_token);
	DegradeCheck(test_readtable);
	DegradeCheck(test_quote);
	DegradeCheck(test_package);
	DegradeCheck(test_lambda);
	DegradeCheck(test_clos);
	DegradeCheck(test_clos_class);
	DegradeCheck(test_clos_type);
	DegradeCheck(test_clos_cache);
	DegradeCheck(test_clos_generic);
	DegradeCheck(test_clos_combination);
	DegradeCheck(test_clos_method);
	DegradeCheck(test_type);
	DegradeCheck(test_type_table);
	DegradeCheck(test_type_symbol);
	DegradeCheck(test_type_parse);
	DegradeCheck(test_type_typep);
	DegradeCheck(test_type_name);
	DegradeCheck(test_type_value);
	DegradeCheck(test_type_object);
	DegradeCheck(test_type_copy);
	DegradeCheck(test_type_optimize);
	DegradeCheck(test_type_range);
	DegradeCheck(test_type_number);
	DegradeCheck(test_type_subtypep);
	DegradeCheck(test_type_upgraded);
	DegradeCheck(test_equal);
	DegradeCheck(test_eval);
	DegradeCheck(test_eval_declare);
	DegradeCheck(test_eval_parse);
	//DegradeCheck(test_eval_optparse); /* error */
	DegradeCheck(test_eval_table);
	DegradeCheck(test_eval_stack);
	DegradeCheck(test_eval_scope);
	DegradeCheck(test_eval_code);
	DegradeCheck(test_integer);
	DegradeCheck(test_bit);
	DegradeCheck(test_format);
	DegradeCheck(test_fmtfloat);
	DegradeCheck(test_print);
	DegradeCheck(test_stream);
	DegradeCheck(test_main);
	DegradeCheck(test_condition);
	DegradeCheck(loadrt);
#if 0
#endif
}

#endif

