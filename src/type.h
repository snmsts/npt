#ifndef __TYPE_HEADER__
#define __TYPE_HEADER__

#include "constant.h"
#include "execute.h"
#include "typedef.h"

enum LISPDECL {
	LISPDECL_EMPTY,
	LISPDECL_OPTIMIZED,
	LISPDECL_SUBTYPEP,
	LISPDECL_TYPE,
	LISPDECL_CLOS,
	LISPDECL_ASTERISK,
	/* Compound-type */
	LISPDECL_AND,
	LISPDECL_OR,
	LISPDECL_EQL,
	LISPDECL_MEMBER,
	LISPDECL_MOD,
	LISPDECL_NOT,
	LISPDECL_SATISFIES,
	LISPDECL_VALUES,
	/* Extract-type */
	LISPDECL_ATOM,
	LISPDECL_LIST,
	LISPDECL_BOOLEAN,
	LISPDECL_VECTOR,
	LISPDECL_SIMPLE_VECTOR,
	LISPDECL_BIT_VECTOR,
	LISPDECL_SIMPLE_BIT_VECTOR,
	LISPDECL_EXTENDED_CHAR,
	LISPDECL_STRING,
	LISPDECL_BASE_STRING,
	LISPDECL_SIMPLE_STRING,
	LISPDECL_SIMPLE_BASE_STRING,
	LISPDECL_SIGNED_BYTE,
	LISPDECL_UNSIGNED_BYTE,
	LISPDECL_BIT,
	LISPDECL_FIXNUM,
	LISPDECL_BIGNUM,
	/* Atomic-type */
	LISPDECL_NIL,
	LISPDECL_T,
	LISPDECL_NULL,
	LISPDECL_CONS,
	LISPDECL_HASH_TABLE,
	LISPDECL_SYMBOL,
	LISPDECL_KEYWORD,
	LISPDECL_PACKAGE,
	LISPDECL_RANDOM_STATE,
	LISPDECL_READTABLE,
	LISPDECL_FUNCTION,
	LISPDECL_COMPILED_FUNCTION,
	LISPDECL_PATHNAME,
	LISPDECL_LOGICAL_PATHNAME,
	LISPDECL_SEQUENCE,
	LISPDECL_ARRAY,
	LISPDECL_SIMPLE_ARRAY,
	LISPDECL_CHARACTER,
	LISPDECL_BASE_CHAR,
	LISPDECL_STANDARD_CHAR,
	LISPDECL_NUMBER,
	LISPDECL_REAL,
	LISPDECL_RATIONAL,
	LISPDECL_RATIO,
	LISPDECL_INTEGER,
	LISPDECL_COMPLEX,
	LISPDECL_FLOAT,
	LISPDECL_SHORT_FLOAT,
	LISPDECL_SINGLE_FLOAT,
	LISPDECL_DOUBLE_FLOAT,
	LISPDECL_LONG_FLOAT,
	LISPDECL_RESTART,
	LISPDECL_ENVIRONMENT,
	LISPDECL_STREAM,
	LISPDECL_BROADCAST_STREAM,
	LISPDECL_CONCATENATED_STREAM,
	LISPDECL_ECHO_STREAM,
	LISPDECL_FILE_STREAM,
	LISPDECL_STRING_STREAM,
	LISPDECL_SYNONYM_STREAM,
	LISPDECL_TWO_WAY_STREAM,
	LISPDECL_QUOTE,
	LISPDECL_SIZE
};

#define RefLispDecl(p)		((enum LISPDECL)(GetUser(p) & 0x7F))
#define GetLispDecl(p,v)	(*(v) = RefLispDecl(p))
#define SetLispDecl(p,v)	SetUser((p), (byte)(v))
#define RefNotDecl(p)		(GetUser(p) & 0x80)
#define GetNotDecl(p,v)		(*(v) = RefNotDecl(p))
#define SetNotDecl(p,v)		setnotdecl_value(p,v)
#define RefArrayType		RefArrayA2
#define GetArrayType		GetArrayA2
#define SetArrayType		SetArrayA2
#define LenArrayType		LenArrayA2

void init_type(void);
void build_type(void);
int type_symbol_p(addr symbol);

void parse_type(addr *ret, addr type);
int typep_clang(addr value, addr type);
int typep_asterisk_clang(addr value, addr type);
int subtypep_clang(addr left, addr right, int *validp);
int subtypep_asterisk_clang(addr left, addr right, int *validp);

#endif

