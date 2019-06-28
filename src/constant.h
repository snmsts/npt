#ifndef __CONSTANT_HEADER__
#define __CONSTANT_HEADER__

#include "build.h"
#include "local.h"
#include "memory.h"

enum CONSTANT_INDEX {
	CONSTANT_EMPTY = 0,

#ifdef LISP_DEGRADE
	CONSTANT_DEBUG1,
	CONSTANT_DEBUG2,
	CONSTANT_DEBUG3,
#endif

	CONSTANT_COMMON_NIL,
	CONSTANT_COMMON_T,

	CONSTANT_PACKAGE_COMMON_LISP,
	CONSTANT_PACKAGE_COMMON_LISP_USER,
	CONSTANT_PACKAGE_KEYWORD,
	CONSTANT_PACKAGE_SYSTEM,
	CONSTANT_PACKAGE_CODE,
	CONSTANT_PACKAGE_CLOS,
	CONSTANT_PACKAGE_RT,
	CONSTANT_PACKAGE_GENTEMP,
	CONSTANT_PACKAGE_DEFAULT_USE,
	CONSTANT_FASL_CODE,
	CONSTANT_FIXNUM_CACHE,
	CONSTANT_FIXNUM_MAX,
	CONSTANT_FIXNUM_MIN,
	CONSTANT_INDEX_MAX,
	CONSTANT_SINGLE_FLOAT_MOST_POSITIVE,
	CONSTANT_SINGLE_FLOAT_MOST_NEGATIVE,
	CONSTANT_SINGLE_FLOAT_LEAST_POSITIVE,
	CONSTANT_SINGLE_FLOAT_LEAST_POSITIVE_NORMALIZED,
	CONSTANT_SINGLE_FLOAT_LEAST_NEGATIVE,
	CONSTANT_SINGLE_FLOAT_LEAST_NEGATIVE_NORMALIZED,
	CONSTANT_SINGLE_FLOAT_EPSILON,
	CONSTANT_SINGLE_FLOAT_NEGATIVE_EPSILON,
	CONSTANT_CHARACTER_CACHE,
	CONSTANT_STREAM_BINARY_TYPE,
	CONSTANT_STREAM_STDIN,
	CONSTANT_STREAM_STDOUT,
	CONSTANT_STREAM_STDERR,
	CONSTANT_CLOSDATA_NO_METHOD,
	CONSTANT_CLOSDATA_SYMBOL_TYPE,

	CONSTANT_ARRAY_T,
	CONSTANT_ARRAY_BIT,
	CONSTANT_ARRAY_CHARACTER,
	CONSTANT_ARRAY_SINGLE_FLOAT,
	CONSTANT_ARRAY_DOUBLE_FLOAT,
	CONSTANT_ARRAY_LONG_FLOAT,
	CONSTANT_ARRAY_SIGNED8,
	CONSTANT_ARRAY_SIGNED16,
	CONSTANT_ARRAY_SIGNED32,
	CONSTANT_ARRAY_UNSIGNED8,
	CONSTANT_ARRAY_UNSIGNED16,
	CONSTANT_ARRAY_UNSIGNED32,
#ifdef LISP_64BIT
	CONSTANT_ARRAY_SIGNED64,
	CONSTANT_ARRAY_UNSIGNED64,
#endif

	CONSTANT_CLOS_T,
	CONSTANT_CLOS_CLASS,
	CONSTANT_CLOS_STANDARD_OBJECT,
	CONSTANT_CLOS_STANDARD_CLASS,
	CONSTANT_CLOS_BUILT_IN_CLASS,
	CONSTANT_CLOS_STRUCTURE_CLASS,
	CONSTANT_CLOS_STRUCTURE_OBJECT,
	CONSTANT_CLOS_FUNCTION,
	CONSTANT_CLOS_GENERIC_FUNCTION,
	CONSTANT_CLOS_STANDARD_GENERIC_FUNCTION,
	CONSTANT_CLOS_METHOD,
	CONSTANT_CLOS_STANDARD_METHOD,
	CONSTANT_CLOS_DEFINE_METHOD_COMBINATION,
	CONSTANT_CLOS_DEFINE_LONG_METHOD_COMBINATION,
	CONSTANT_CLOS_DEFINE_SHORT_METHOD_COMBINATION,
	CONSTANT_CLOS_LONG_METHOD_COMBINATION,
	CONSTANT_CLOS_SHORT_METHOD_COMBINATION,
	CONSTANT_CLOS_METHOD_COMBINATION,
	CONSTANT_CLOS_EQL_SPECIALIZER,
	CONSTANT_CLOS_FUNCALLABLE_STANDARD_OBJECT,
	CONSTANT_CLOS_FUNCALLABLE_STANDARD_CLASS,
	CONSTANT_CLOS_FORWARD_REFERENCED_CLASS,

	CONSTANT_CLOS_ARRAY,
	CONSTANT_CLOS_CHARACTER,
	CONSTANT_CLOS_CONDITION,
	CONSTANT_CLOS_HASH_TABLE,
	CONSTANT_CLOS_NUMBER,
	CONSTANT_CLOS_PACKAGE,
	CONSTANT_CLOS_PATHNAME,
	CONSTANT_CLOS_RANDOM_STATE,
	CONSTANT_CLOS_READTABLE,
	CONSTANT_CLOS_RESTART,
	CONSTANT_CLOS_SEQUENCE,
	CONSTANT_CLOS_STREAM,
	CONSTANT_CLOS_SYMBOL,
	CONSTANT_CLOS_LOGICAL_PATHNAME,

	CONSTANT_CLOS_LIST,
	CONSTANT_CLOS_CONS,
	CONSTANT_CLOS_VECTOR,
	CONSTANT_CLOS_BIT_VECTOR,
	CONSTANT_CLOS_NULL,
	CONSTANT_CLOS_STRING,
	CONSTANT_CLOS_COMPLEX,
	CONSTANT_CLOS_REAL,
	CONSTANT_CLOS_FLOAT,
	CONSTANT_CLOS_RATIONAL,
	CONSTANT_CLOS_INTEGER,
	CONSTANT_CLOS_RATIO,
	CONSTANT_CLOS_BROADCAST_STREAM,
	CONSTANT_CLOS_CONCATENATED_STREAM,
	CONSTANT_CLOS_ECHO_STREAM,
	CONSTANT_CLOS_FILE_STREAM,
	CONSTANT_CLOS_STRING_STREAM,
	CONSTANT_CLOS_SYNONYM_STREAM,
	CONSTANT_CLOS_TWO_WAY_STREAM,
	CONSTANT_CLOS_PROMPT_STREAM,
	CONSTANT_CLOS_BASE_CHAR,
	CONSTANT_CLOS_EXTENDED_CHAR,
	CONSTANT_CLOS_STANDARD_CHAR,
	CONSTANT_CLOS_SIMPLE_ARRAY,
	CONSTANT_CLOS_SIMPLE_VECTOR,
	CONSTANT_CLOS_BASE_STRING,
	CONSTANT_CLOS_SIMPLE_STRING,
	CONSTANT_CLOS_SIMPLE_BASE_STRING,
	CONSTANT_CLOS_SIMPLE_BIT_VECTOR,
	CONSTANT_CLOS_BIGNUM,
	CONSTANT_CLOS_FIXNUM,
	CONSTANT_CLOS_SHORT_FLOAT,
	CONSTANT_CLOS_SINGLE_FLOAT,
	CONSTANT_CLOS_DOUBLE_FLOAT,
	CONSTANT_CLOS_LONG_FLOAT,
	CONSTANT_CLOS_SIGNED_BYTE,
	CONSTANT_CLOS_UNSIGNED_BYTE,
	CONSTANT_CLOS_BIT,
	CONSTANT_CLOS_COMPILED_FUNCTION,
	CONSTANT_CLOS_KEYWORD,

	CONSTANT_CLOS_SLOT_DEFINITION,
	CONSTANT_CLOS_STANDARD_SLOT_DEFINITION,
	CONSTANT_CLOS_COMBINATION_STANDARD,

	CONSTANT_CONDITION_ARITHMETIC_ERROR,
	CONSTANT_CONDITION_CELL_ERROR,
	CONSTANT_CONDITION_CONTROL_ERROR,
	CONSTANT_CONDITION_DIVISION_BY_ZERO,
	CONSTANT_CONDITION_END_OF_FILE,
	CONSTANT_CONDITION_ERROR,
	CONSTANT_CONDITION_FILE_ERROR,
	CONSTANT_CONDITION_FLOATING_POINT_INEXACT,
	CONSTANT_CONDITION_FLOATING_POINT_INVALID_OPERATION,
	CONSTANT_CONDITION_FLOATING_POINT_OVERFLOW,
	CONSTANT_CONDITION_FLOATING_POINT_UNDERFLOW,
	CONSTANT_CONDITION_PACKAGE_ERROR,
	CONSTANT_CONDITION_PARSE_ERROR,
	CONSTANT_CONDITION_PRINT_NOT_READABLE,
	CONSTANT_CONDITION_PROGRAM_ERROR,
	CONSTANT_CONDITION_READER_ERROR,
	CONSTANT_CONDITION_SERIOUS_CONDITION,
	CONSTANT_CONDITION_SIMPLE_CONDITION,
	CONSTANT_CONDITION_SIMPLE_ERROR,
	CONSTANT_CONDITION_SIMPLE_TYPE_ERROR,
	CONSTANT_CONDITION_SIMPLE_WARNING,
	CONSTANT_CONDITION_STORAGE_CONDITION,
	CONSTANT_CONDITION_STREAM_ERROR,
	CONSTANT_CONDITION_STYLE_WARNING,
	CONSTANT_CONDITION_TYPE_ERROR,
	CONSTANT_CONDITION_UNBOUND_SLOT,
	CONSTANT_CONDITION_UNBOUND_VARIABLE,
	CONSTANT_CONDITION_UNDEFINED_FUNCTION,
	CONSTANT_CONDITION_WARNING,
	CONSTANT_CONDITION_SAVECORE,

#include "intern_const.h"
	CONSTANT_SIZE
};

typedef enum CONSTANT_INDEX constindex;

#define RetConstant(i)		RetArrayA4(Root(LISPINDEX_CONST),(i))
#define GetConstant(i,v)	GetArrayA4(Root(LISPINDEX_CONST),(i),(v))
#define SetConstant(i,v)	SetArrayA4(Root(LISPINDEX_CONST),(i),(v))

#define GetConst(i,v)		GetConstant(CONSTANT_##i, (v))
#define SetConst(i,v)		SetConstant(CONSTANT_##i, (v))

void build_constant(void);
void intern_symbol_header(void);
void specialconstant(enum CONSTANT_INDEX index, const char *package, const char *name);
void gensymconstant(enum CONSTANT_INDEX index, const char *name);
void keywordconstant(enum CONSTANT_INDEX index, const char *name);
void commonconstant(enum CONSTANT_INDEX index, const char *name);

void symbolchar_common(enum CONSTANT_INDEX index, const char *name);
void symbolchar_keyword(enum CONSTANT_INDEX index, const char *name);
void quotelist_heap(addr *ret, addr name);
void pushconst_heap(addr *ret, constindex index);
#define PushConst(a,b) pushconst_heap((a),CONSTANT_##b)

#endif

