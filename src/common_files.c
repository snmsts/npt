/*
 *  ANSI COMMON LISP: 20. Files
 */
#include "common_header.h"
#include "cons.h"
#include "files.h"
#include "pathname.h"
#include "type_parse.h"

/* (defun directory (pathname &key) ...) -> list */
static void function_directory(Execute ptr, addr pos)
{
	directory_files(ptr, &pos, pos);
	setresult_control(ptr, pos);
}

static void type_directory(addr *ret)
{
	addr arg, values;

	GetCallType(&arg, PathnameDesigner);
	var1_argtype(&arg, arg);
	GetCallType(&values, Values_List);
	type_compiled_heap(arg, values, ret);
}

static void defun_directory(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_DIRECTORY, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_directory);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_directory(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun probe-file (pathspec) ...) -> truename */
static void function_probe_file(Execute ptr, addr pos)
{
	probe_file_files(ptr, &pos, pos);
	setresult_control(ptr, pos);
}

static void defun_probe_file(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_PROBE_FILE, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_probe_file);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetCallType(&type, Compiled_Pathname);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun ensure-directories-exist (pathspec &key verbose) ...)
 *     -> pathspec, created
 *   pathspec  pathname-designer
 *   verbose   t  ;; boolean
 *   created   boolean
 */
static void function_ensure_directories_exist(Execute ptr, addr pos, addr rest)
{
	if (getkeyargs(rest, KEYWORD_VERBOSE, &rest)) rest = Nil;
	ensure_directories_exist_files(ptr, &pos, &rest, pos, rest != Nil);
	setvalues_va_control(ptr, pos, rest, NULL);
}

static void type_ensure_directories_exist(addr *ret)
{
	addr arg, values, type;

	/* key */
	KeyCallType(&type, VERBOSE, T);
	conscar_heap(&type, type);
	/* type */
	GetCallType(&arg, PathnameDesigner);
	var1key_argtype(&arg, arg, type);
	GetCallType(&values, Pathname);
	GetCallType(&type, Boolean);
	values2_valuestype(&values, values, type);
	type_compiled_heap(arg, values, ret);
}

static void defun_ensure_directories_exist(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_ENSURE_DIRECTORIES_EXIST, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1dynamic(pos, function_ensure_directories_exist);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_ensure_directories_exist(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun truename (pathspec) ...) -> pathname */
static void function_truename(Execute ptr, addr pos)
{
	physical_pathname_heap(ptr, pos, &pos);
	setresult_control(ptr, pos);
}

static void defun_truename(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_TRUENAME, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_truename);
	SetFunctionCommon(symbol, pos);
	/* type */
	GetCallType(&type, Compiled_Pathname);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun file-author (pathspec) ...) -> (or string null) */
static void function_file_author(Execute ptr, addr pos)
{
	file_author_files(ptr, &pos, pos);
	setresult_control(ptr, pos);
}

static void type_file_author(addr *ret)
{
	addr arg, values;

	GetCallType(&arg, PathnameDesigner);
	var1_argtype(&arg, arg);
	GetCallType(&values, Values_StringNull);
	type_compiled_heap(arg, values, ret);
}

static void defun_file_author(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FILE_AUTHOR, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_file_author);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_file_author(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun file-write-date (pathspec) ...) -> (or integer null) */
static void function_file_write_date(Execute ptr, addr pos)
{
	file_write_date_files(ptr, &pos, pos);
	setresult_control(ptr, pos);
}

static void type_file_write_date(addr *ret)
{
	addr arg, values;

	GetCallType(&arg, PathnameDesigner);
	var1_argtype(&arg, arg);
	GetCallType(&values, IntegerNull);
	result_valuestype(&values, values);
	type_compiled_heap(arg, values, ret);
}

static void defun_file_write_date(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FILE_WRITE_DATE, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_file_write_date);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_file_write_date(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun rename-file (file rename) ...) -> new from to */
static void function_rename_file(Execute ptr, addr file1, addr file2)
{
	addr file3;
	rename_file_files(ptr, &file1, &file2, &file3, file1, file2);
	setvalues_va_control(ptr, file1, file2, file3, NULL);
}

static void type_rename_file(addr *ret)
{
	addr arg, values;

	GetCallType(&values, PathnameDesigner);
	var2_argtype(&arg, values, values);
	values3_valuestype(&values, values, values, values);
	type_compiled_heap(arg, values, ret);
}

static void defun_rename_file(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_RENAME_FILE, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var2(pos, function_rename_file);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_rename_file(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun delete-file (file) ...) -> (eql t) */
static void function_delete_file(Execute ptr, addr pos)
{
	delete_file_files(ptr, pos);
	setresult_control(ptr, T);
}

static void type_delete_file(addr *ret)
{
	addr arg, values;

	GetCallType(&arg, PathnameDesigner);
	var1_argtype(&arg, arg);
	GetCallType(&values, Values_EqlT);
	type_compiled_heap(arg, values, ret);
}

static void defun_delete_file(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_DELETE_FILE, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_delete_file);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_delete_file(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/* (defun file-error-pathname (condition) ...) -> pathname-designer */
static void function_file_error_pathname(Execute ptr, addr var)
{
	file_error_pathname(&var, var);
	setresult_control(ptr, var);
}

static void type_file_error_pathname(addr *ret)
{
	addr arg, values;

	GetCallType(&arg, FileError);
	var1_argtype(&arg, arg);
	GetCallType(&values, PathnameDesigner);
	result_valuestype(&values, values);
	type_compiled_heap(arg, values, ret);
}

static void defun_file_error_pathname(void)
{
	addr symbol, pos, type;

	/* function */
	GetConst(COMMON_FILE_ERROR_PATHNAME, &symbol);
	compiled_heap(&pos, symbol);
	setcompiled_var1(pos, function_file_error_pathname);
	SetFunctionCommon(symbol, pos);
	/* type */
	type_file_error_pathname(&type);
	settype_function(pos, type);
	settype_function_symbol(symbol, type);
}


/*
 *  intern
 */
void intern_common_files(void)
{
	defun_directory();
	defun_probe_file();
	defun_ensure_directories_exist();
	defun_truename();
	defun_file_author();
	defun_file_write_date();
	defun_rename_file();
	defun_delete_file();
	defun_file_error_pathname();
}
