#include "type_number.c"
#include "calltype.h"
#include "character.h"
#include "clos.h"
#include "common.h"
#include "condition.h"
#include "degrade.h"
#include "equal.h"
#include "hashtable.h"
#include "package.h"
#include "pathname.h"
#include "random_state.h"
#include "readtable.h"
#include "sequence.h"
#include "stream.h"
#include "syscall.h"
#include "symbol.h"

static void parse_type_string(addr *ret, const char *code)
{
	readstring(ret, code);
	parse_type_heap(ret, *ret);
}


/*
 *  real_filter
 */
static int test_type_range_left(void)
{
	addr pos, check;

	type_range_left(NULL, &pos, LISPDECL_INTEGER, T, fixnum_heapr(10));
	test(range_left_p(pos), "type_range_left1");
	range_left_value(pos, &check, &pos);
	test(check == T, "type_range_left2");
	test(RefFixnum(pos) == 10, "type_range_left3");

	RETURN;
}

static int test_type_range_right(void)
{
	addr pos, check;

	type_range_right(NULL, &pos, LISPDECL_INTEGER, T, fixnum_heapr(10));
	test(range_right_p(pos), "type_range_right1");
	range_right_value(pos, &check, &pos);
	test(check == T, "type_range_right2");
	test(RefFixnum(pos) == 10, "type_range_right3");

	RETURN;
}

static int test_type_range_not(void)
{
	addr pos, left, right;

	type_range_not(NULL, &pos, LISPDECL_INTEGER,
			Nil, fixnum_heapr(10),
			T, fixnum_heapr(20));
	test(RefLispDecl(pos) == LISPDECL_OR, "type_range_not1");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 2, "type_range_not2");

	GetArrayA4(pos, 0, &left);
	GetArrayA4(pos, 1, &right);
	test(RefLispDecl(left) == LISPDECL_INTEGER, "type_range_not3");
	test(RefLispDecl(right) == LISPDECL_INTEGER, "type_range_not4");
	test(range_left_p(left), "type_range_not5");
	test(range_right_p(right), "type_range_not6");

	range_left_value(left, &pos, &left);
	test(pos == Nil, "type_range_not7");
	test(RefFixnum(left) == 20, "type_range_not8");

	range_right_value(right, &pos, &right);
	test(pos == T, "type_range_not9");
	test(RefFixnum(right) == 10, "type_range_not10");

	RETURN;
}

static int equal_real_asterisk(addr left, addr right)
{
	int aster1, aster2;

	aster1 = asterisk_p(left);
	aster2 = asterisk_p(right);
	if (aster1 && aster2) return 1;
	if (aster1) return 0;
	if (aster2) return 0;
	return eql_function(left, right);
}

static int equal_real(addr left, addr right)
{
	addr check1, check2;

	if (! range_type_p(left)) return 0;
	if (! range_type_p(right)) return 0;
	if (RefLispDecl(left) != RefLispDecl(right)) return 0;
	if (RefNotDecl(left) != RefNotDecl(right)) return 0;

	GetArrayType(left, 0, &check1);
	GetArrayType(right, 0, &check2);
	if (! equal_real_asterisk(check1, check2)) return 0;
	GetArrayType(left, 1, &check1);
	GetArrayType(right, 1, &check2);
	if (! equal_real_asterisk(check1, check2)) return 0;
	GetArrayType(left, 2, &check1);
	GetArrayType(right, 2, &check2);
	if (! equal_real_asterisk(check1, check2)) return 0;
	GetArrayType(left, 3, &check1);
	GetArrayType(right, 3, &check2);
	if (! equal_real_asterisk(check1, check2)) return 0;

	return 1;
}

static int test_real_filter_not_range(void)
{
	addr pos1, pos2;

	parse_type_string(&pos1, "integer");
	real_filter_not_range(NULL, &pos1, pos1, LISPDECL_REAL);
	test(pos1 == Nil, "real_filter_not_range1");

	parse_type_string(&pos1, "(integer 10 *)");
	parse_type_string(&pos2, "(real * (10))");
	real_filter_not_range(NULL, &pos1, pos1, LISPDECL_REAL);
	test(equal_real(pos1, pos2), "real_filter_not_range2");

	parse_type_string(&pos1, "(integer * (20))");
	parse_type_string(&pos2, "(real 20 *)");
	real_filter_not_range(NULL, &pos1, pos1, LISPDECL_REAL);
	test(equal_real(pos1, pos2), "real_filter_not_range3");

	parse_type_string(&pos1, "(integer 10 (20))");
	real_filter_not_range(NULL, &pos1, pos1, LISPDECL_REAL);
	test(RefLispDecl(pos1) == LISPDECL_OR, "real_filter_not_range4");

	RETURN;
}

static int test_real_filter_not(void)
{
	addr pos1, pos2;

	parse_type_string(&pos1, "integer");
	real_filter_not(NULL, &pos1, pos1, LISPDECL_INTEGER);
	test(pos1 == Nil, "real_filter_not1");

	parse_type_string(&pos1, "integer");
	real_filter_not(NULL, &pos1, pos1, LISPDECL_RATIONAL);
	parse_type_string(&pos2, "rational");
	test(equal_real(pos1, pos2), "real_filter_not2");

	parse_type_string(&pos1, "real");
	real_filter_not(NULL, &pos1, pos1, LISPDECL_INTEGER);
	test(pos1 == Nil, "real_filter_not3");

	parse_type_string(&pos1, "(integer 10)");
	real_filter_not(NULL, &pos1, pos1, LISPDECL_INTEGER);
	parse_type_string(&pos2, "(integer * (10))");
	test(equal_real(pos1, pos2), "real_filter_not4");

	parse_type_string(&pos1, "(integer 10)");
	real_filter_not(NULL, &pos1, pos1, LISPDECL_RATIONAL);
	parse_type_string(&pos2, "rational");
	test(equal_real(pos1, pos2), "real_filter_not5");

	parse_type_string(&pos1, "(real 10)");
	real_filter_not(NULL, &pos1, pos1, LISPDECL_RATIONAL);
	parse_type_string(&pos2, "(rational * (10))");
	test(equal_real(pos1, pos2), "real_filter_not6");

	RETURN;
}

static int test_real_filter_normal(void)
{
	addr pos1, pos2;

	parse_type_string(&pos1, "integer");
	real_filter_normal(NULL, &pos1, pos1, LISPDECL_INTEGER);
	parse_type_string(&pos2, "integer");
	test(equal_real(pos1, pos2), "real_filter_normal1");

	parse_type_string(&pos1, "integer");
	real_filter_normal(NULL, &pos1, pos1, LISPDECL_RATIONAL);
	test(pos1 == Nil, "real_filter_normal2");

	parse_type_string(&pos1, "real");
	real_filter_normal(NULL, &pos1, pos1, LISPDECL_INTEGER);
	parse_type_string(&pos2, "integer");
	test(equal_real(pos1, pos2), "real_filter_normal3");

	parse_type_string(&pos1, "(real 10 20)");
	real_filter_normal(NULL, &pos1, pos1, LISPDECL_INTEGER);
	parse_type_string(&pos2, "(integer 10 20)");
	test(equal_real(pos1, pos2), "real_filter_normal4");

	RETURN;
}

static int test_real_filter_type(void)
{
	addr pos1, pos2;

	parse_type_string(&pos1, "integer");
	real_filter_type(NULL, &pos1, pos1, LISPDECL_INTEGER);
	parse_type_string(&pos2, "integer");
	test(equal_real(pos1, pos2), "real_filter_type1");

	parse_type_string(&pos1, "integer");
	SetNotDecl(pos1, 1);
	real_filter_type(NULL, &pos1, pos1, LISPDECL_INTEGER);
	test(pos1 == Nil, "real_filter_type2");

	parse_type_string(&pos1, "real");
	real_filter_type(NULL, &pos1, pos1, LISPDECL_INTEGER);
	parse_type_string(&pos2, "integer");
	test(equal_real(pos1, pos2), "real_filter_type3");

	parse_type_string(&pos1, "real");
	SetNotDecl(pos1, 1);
	real_filter_type(NULL, &pos1, pos1, LISPDECL_INTEGER);
	test(pos1 == Nil, "real_filter_type4");

	parse_type_string(&pos1, "integer");
	real_filter_type(NULL, &pos1, pos1, LISPDECL_REAL);
	test(pos1 == Nil, "real_filter_type5");

	parse_type_string(&pos1, "integer");
	SetNotDecl(pos1, 1);
	real_filter_type(NULL, &pos1, pos1, LISPDECL_REAL);
	parse_type_string(&pos2, "real");
	test(equal_real(pos1, pos2), "real_filter_type6");

	RETURN;
}

static int test_real_filter_and(void)
{
	addr pos, check;

	parse_type_string(&pos, "(and real rational real)");
	real_filter_and(NULL, &pos, pos, LISPDECL_REAL);
	test(RefLispDecl(pos) == LISPDECL_AND, "real_filter_and1");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 1, "real_filter_and2");
	GetArrayA4(pos, 0, &pos);
	test(RefLispDecl(pos) == LISPDECL_REAL, "real_filter_and3");

	parse_type_string(&pos, "(and real rational)");
	real_filter_and(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_AND, "real_filter_and4");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 2, "real_filter_and5");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter_and6");
	GetArrayA4(pos, 1, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter_and7");

	RETURN;
}

static int test_real_filter_or(void)
{
	addr pos, check;

	parse_type_string(&pos, "(or real rational)");
	real_filter_or(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_OR, "real_filter_or1");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 2, "real_filter_or2");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter_or3");
	GetArrayA4(pos, 1, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter_or4");

	parse_type_string(&pos, "(or real rational real)");
	real_filter_or(NULL, &pos, pos, LISPDECL_REAL);
	test(RefLispDecl(pos) == LISPDECL_OR, "real_filter_or5");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 2, "real_filter_or6");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_REAL, "real_filter_or7");
	GetArrayA4(pos, 1, &check);
	test(RefLispDecl(check) == LISPDECL_REAL, "real_filter_or8");

	parse_type_string(&pos, "(or real rational)");
	real_filter_or(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_OR, "real_filter_or9");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 2, "real_filter_or10");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter_or11");
	GetArrayA4(pos, 1, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter_or12");

	RETURN;
}

static int test_real_filter(void)
{
	addr pos, check;

	parse_type_string(&pos, "real");
	real_filter(NULL, &pos, pos, LISPDECL_INTEGER);
	parse_type_string(&check, "integer");
	test(equal_real(pos, check), "real_filter1");

	parse_type_string(&pos, "integer");
	real_filter(NULL, &pos, pos, LISPDECL_REAL);
	test(pos == Nil, "real_filter2");

	parse_type_string(&pos, "(and real)");
	real_filter(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_AND, "real_filter3");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 1, "real_filter4");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter5");

	parse_type_string(&pos, "(or real)");
	real_filter(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_OR, "real_filter5");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 1, "real_filter6");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "real_filter7");

	RETURN;
}


/*
 *  merge_range
 */
static int test_merge_range_cons(void)
{
	addr pos;

	parse_type_string(&pos, "integer");
	merge_range_cons(NULL, &pos, pos, LISPDECL_INTEGER);
	test(singlep(pos), "merge_range_cons1");
	GetCar(pos, &pos);
	test(RefLispDecl(pos) == LISPDECL_INTEGER, "merge_range_cons2");

	RETURN;
}


/*
 *  merge-range-and
 */
static int test_make_range_left_right(void)
{
	addr left, right;

	parse_type_string(&left, "(integer 10 20)");
	parse_type_string(&right, "(real (30) (40))");
	make_range_left_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 (40))");
	test(equal_real(left, right), "make_range_left_right1");

	RETURN;
}

static int test_make_range_left_aster(void)
{
	addr left, right;

	parse_type_string(&left, "(integer 10 20)");
	make_range_left_aster(NULL, &left, left);
	parse_type_string(&right, "(integer 10 *)");
	test(equal_real(left, right), "make_range_left_aster1");

	RETURN;
}

static int test_make_range_aster_right(void)
{
	addr left, right;

	parse_type_string(&left, "(integer (10) (20))");
	make_range_aster_right(NULL, &left, left);
	parse_type_string(&right, "(integer * (20))");
	test(equal_real(left, right), "make_range_aster_right1");

	RETURN;
}

static int test_range_and_left_left(void)
{
	addr left, right, check;

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer 20 *)");
	range_and_left_left(&check, left, right);
	test(check == right, "range_and_left_left1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_left_left(&check, left, right);
	test(check == left, "range_and_left_left2");

	parse_type_string(&left, "(integer (10) *)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_left_left(&check, left, right);
	test(check == left, "range_and_left_left3");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer (10) *)");
	range_and_left_left(&check, left, right);
	test(check == right, "range_and_left_left4");

	RETURN;
}

static int test_range_and_right_left(void)
{
	addr left, right;

	parse_type_string(&left, "(integer * 20)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_right_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 20)");
	test(equal_real(left, right), "range_and_right_left1");

	parse_type_string(&left, "(integer * 5)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_right_left(NULL, &left, left, right);
	test(left == Nil, "range_and_right_left2");

	parse_type_string(&left, "(integer * 10)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_right_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 10)");
	test(equal_real(left, right), "range_and_right_left3");

	parse_type_string(&left, "(integer * (10))");
	parse_type_string(&right, "(integer 10 *)");
	range_and_right_left(NULL, &left, left, right);
	test(left == Nil, "range_and_right_left4");

	parse_type_string(&left, "(integer * 10)");
	parse_type_string(&right, "(integer (10) *)");
	range_and_right_left(NULL, &left, left, right);
	test(left == Nil, "range_and_right_left5");

	parse_type_string(&left, "(integer * (10))");
	parse_type_string(&right, "(integer (10) *)");
	range_and_right_left(NULL, &left, left, right);
	test(left == Nil, "range_and_right_left6");

	RETURN;
}

static int test_range_and_between_left(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_between_left(NULL, &check, left, right);
	test(check == left, "range_and_between_left1");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 30 *)");
	range_and_between_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 30 40)");
	test(equal_real(left, right), "range_and_between_left2");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 50 *)");
	range_and_between_left(NULL, &check, left, right);
	test(check == Nil, "range_and_between_left3");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 20 *)");
	range_and_between_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 40)");
	test(equal_real(left, right), "range_and_between_left4");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer 20 *)");
	range_and_between_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer (20) 40)");
	test(equal_real(left, right), "range_and_between_left5");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer (20) *)");
	range_and_between_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer (20) 40)");
	test(equal_real(left, right), "range_and_between_left6");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer (20) *)");
	range_and_between_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer (20) 40)");
	test(equal_real(left, right), "range_and_between_left7");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 40 *)");
	range_and_between_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 40 40)");
	test(equal_real(left, right), "range_and_between_left8");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer 40 *)");
	range_and_between_left(NULL, &check, left, right);
	test(check == Nil, "range_and_between_left9");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer (40) *)");
	range_and_between_left(NULL, &check, left, right);
	test(check == Nil, "range_and_between_left10");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer (40) *)");
	range_and_between_left(NULL, &check, left, right);
	test(check == Nil, "range_and_between_left11");

	RETURN;
}

static int test_range_and_left(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer * *)");
	parse_type_string(&right, "(integer 20 *)");
	range_and_left(NULL, &check, left, right);
	test(check == right, "range_and_left1");

	parse_type_string(&left, "(integer 40 *)");
	parse_type_string(&right, "(integer 20 *)");
	range_and_left(NULL, &check, left, right);
	test(check == left, "range_and_left2");

	parse_type_string(&left, "(integer * 10)");
	parse_type_string(&right, "(integer 20 *)");
	range_and_left(NULL, &check, left, right);
	test(check == Nil, "range_and_left3");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 *)");
	range_and_left(NULL, &check, left, right);
	test(check == left, "range_and_left4");

	RETURN;
}

static int test_range_and_left_right(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer * 20)");
	range_and_left_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 20)");
	test(equal_real(left, right), "range_and_left_right1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer * 10)");
	range_and_left_right(NULL, &check, left, right);
	test(check == Nil, "range_and_left_right2");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer * 10)");
	range_and_left_right(NULL, &check, left, right);
	range_and_left_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 10)");
	test(equal_real(left, right), "range_and_left_right3");

	parse_type_string(&left, "(integer (10) *)");
	parse_type_string(&right, "(integer * 10)");
	range_and_left_right(NULL, &check, left, right);
	test(check == Nil, "range_and_left_right4");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer * (10))");
	range_and_left_right(NULL, &check, left, right);
	test(check == Nil, "range_and_left_right5");

	parse_type_string(&left, "(integer (10) *)");
	parse_type_string(&right, "(integer * (10))");
	range_and_left_right(NULL, &check, left, right);
	test(check == Nil, "range_and_left_right6");

	RETURN;
}

static int test_range_and_right_right(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer * 10)");
	parse_type_string(&right, "(integer * 20)");
	range_and_right_right(&check, left, right);
	test(check == left, "range_and_right_right1");

	parse_type_string(&left, "(integer * 20)");
	parse_type_string(&right, "(integer * 10)");
	range_and_right_right(&check, left, right);
	test(check == right, "range_and_right_right2");

	parse_type_string(&left, "(integer * (10))");
	parse_type_string(&right, "(integer * 10)");
	range_and_right_right(&check, left, right);
	test(check == left, "range_and_right_right3");

	parse_type_string(&left, "(integer * 10)");
	parse_type_string(&right, "(integer * (10))");
	range_and_right_right(&check, left, right);
	test(check == right, "range_and_right_right4");

	RETURN;
}

static int test_range_and_between_right(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 50)");
	range_and_between_right(NULL, &check, left, right);
	test(check == left, "range_and_between_right1");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 30)");
	range_and_between_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 30)");
	test(equal_real(left, right), "range_and_between_right2");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 10)");
	range_and_between_right(NULL, &check, left, right);
	test(check == Nil, "range_and_between_right3");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 20)");
	range_and_between_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 20)");
	test(equal_real(left, right), "range_and_between_right4");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer * 20)");
	range_and_between_right(NULL, &check, left, right);
	test(check == Nil, "range_and_between_right5");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * (20))");
	range_and_between_right(NULL, &check, left, right);
	test(check == Nil, "range_and_between_right6");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer * (20))");
	range_and_between_right(NULL, &check, left, right);
	test(check == Nil, "range_and_between_right7");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 40)");
	range_and_between_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 40)");
	test(equal_real(left, right), "range_and_between_right8");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer * 40)");
	range_and_between_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 (40))");
	test(equal_real(left, right), "range_and_between_right9");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * (40))");
	range_and_between_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 (40))");
	test(equal_real(left, right), "range_and_between_right10");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer * (40))");
	range_and_between_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 (40))");
	test(equal_real(left, right), "range_and_between_right11");

	RETURN;
}

static int test_range_and_right(void)
{
	addr check, left, right;

	parse_type_string(&left, "integer");
	parse_type_string(&right, "(integer * 10)");
	range_and_right(NULL, &check, left, right);
	test(check == right, "range_and_right1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer * 10)");
	range_and_right(NULL, &check, left, right);
	test(check == Nil, "range_and_right2");

	parse_type_string(&left, "(integer * 20)");
	parse_type_string(&right, "(integer * 10)");
	range_and_right(NULL, &check, left, right);
	test(check == right, "range_and_right3");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 30)");
	range_and_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 30)");
	test(equal_real(left, right), "range_and_right4");

	RETURN;
}

static int test_range_and_between_between(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer 10 50)");
	parse_type_string(&right, "(integer 20 30)");
	range_and_between_between(NULL, &check, left, right);
	test(check == right, "range_and_between_between1");

	parse_type_string(&left, "(integer 10 50)");
	parse_type_string(&right, "(integer 20 30)");
	range_and_between_between(NULL, &check, right, left);
	test(check == right, "range_and_between_between2");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 30 50)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 30 40)");
	test(equal_real(left, right), "range_and_between_between3");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 30)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 30)");
	test(equal_real(left, right), "range_and_between_between4");

	parse_type_string(&left, "(integer 21 40)");
	parse_type_string(&right, "(integer 10 20)");
	range_and_between_between(NULL, &check, right, left);
	test(check == Nil, "range_and_between_between5");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 20)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 20)");
	test(equal_real(left, right), "range_and_between_between6");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer 10 20)");
	range_and_between_between(NULL, &check, right, left);
	test(check == Nil, "range_and_between_between7");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 (20))");
	range_and_between_between(NULL, &check, right, left);
	test(check == Nil, "range_and_between_between8");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer 10 (20))");
	range_and_between_between(NULL, &check, right, left);
	test(check == Nil, "range_and_between_between9");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 20 30)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 30)");
	test(equal_real(left, right), "range_and_between_between10");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer 20 30)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer (20) 30)");
	test(equal_real(left, right), "range_and_between_between11");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer (20) 30)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer (20) 30)");
	test(equal_real(left, right), "range_and_between_between12");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer (20) 30)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer (20) 30)");
	test(equal_real(left, right), "range_and_between_between13");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 30 40)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 30 40)");
	test(equal_real(left, right), "range_and_between_between14");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer 30 40)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 30 (40))");
	test(equal_real(left, right), "range_and_between_between15");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 30 (40))");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 30 (40))");
	test(equal_real(left, right), "range_and_between_between16");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer 30 (40))");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 30 (40))");
	test(equal_real(left, right), "range_and_between_between17");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 40 50)");
	range_and_between_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 40 40)");
	test(equal_real(left, right), "range_and_between_between18");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer 40 50)");
	range_and_between_between(NULL, &check, left, right);
	test(check == Nil, "range_and_between_between19");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer (40) 50)");
	range_and_between_between(NULL, &check, left, right);
	test(check == Nil, "range_and_between_between20");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer (40) 50)");
	range_and_between_between(NULL, &check, left, right);
	test(check == Nil, "range_and_between_between21");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 41 50)");
	range_and_between_between(NULL, &check, left, right);
	test(check == Nil, "range_and_between_between22");

	RETURN;
}

static int test_range_and_between(void)
{
	addr check, left, right;

	parse_type_string(&left, "integer");
	parse_type_string(&right, "(integer 10 20)");
	range_and_between(NULL, &check, left, right);
	test(check == right, "range_and_between1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer 10 30)");
	range_and_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 20 30)");
	test(equal_real(left, right), "range_and_between2");

	parse_type_string(&left, "(integer * 20)");
	parse_type_string(&right, "(integer 10 30)");
	range_and_between(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 20)");
	test(equal_real(left, right), "range_and_between3");

	parse_type_string(&left, "(integer 15 16)");
	parse_type_string(&right, "(integer 10 20)");
	range_and_between(NULL, &check, left, right);
	test(check == left, "range_and_between4");

	RETURN;
}

static int test_range_and(void)
{
	addr check, left, right;

	parse_type_string(&left, "(integer 10 20)");
	parse_type_string(&right, "integer");
	range_and(NULL, &check, left, right);
	test(check == left, "range_and1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer 10 *)");
	range_and(NULL, &check, left, right);
	test(check == left, "range_and2");

	parse_type_string(&left, "(integer * 10)");
	parse_type_string(&right, "(integer * 20)");
	range_and(NULL, &check, left, right);
	test(check == left, "range_and3");

	parse_type_string(&left, "(integer 15 16)");
	parse_type_string(&right, "(integer 10 20)");
	range_and(NULL, &check, left, right);
	test(check == left, "range_and4");

	RETURN;
}

static int test_map_range_and(void)
{
	addr left, right, pos;

	parse_type_string(&left, "(integer 10 20)");
	parse_type_string(&pos, "(integer 30 40)");
	parse_type_string(&right, "(integer 50 *)");
	list_heap(&left, left, pos, NULL);
	map_range_and(NULL, &pos, left, right);
	test(pos == Nil, "map_range_and1");

	parse_type_string(&left, "(integer 10 20)");
	parse_type_string(&pos, "(integer 30 40)");
	parse_type_string(&right, "(integer 35 *)");
	list_heap(&left, left, pos, NULL);
	map_range_and(NULL, &left, left, right);
	test(singlep(left), "map_range_and2");
	GetCar(left, &left);
	parse_type_string(&right, "(integer 35 40)");
	test(equal_real(left, right), "map_range_and3");

	RETURN;
}

static int test_merge_range_andplus(void)
{
	addr left, right, pos;

	parse_type_string(&left, "(integer 10 20)");
	parse_type_string(&pos, "(integer 30 40)");
	parse_type_string(&right, "(integer 50 *)");
	conscar_heap(&right, right);
	list_heap(&left, left, pos, NULL);
	merge_range_andplus(NULL, &pos, left, right);
	test(pos == Nil, "merge_range_andplus1");

	parse_type_string(&left, "(integer 10 20)");
	parse_type_string(&pos, "(integer 30 40)");
	parse_type_string(&right, "(integer 35 *)");
	conscar_heap(&right, right);
	list_heap(&left, left, pos, NULL);
	merge_range_andplus(NULL, &left, left, right);
	test(singlep(left), "merge_range_andplus2");
	GetCar(left, &left);
	parse_type_string(&right, "(integer 35 40)");
	test(equal_real(left, right), "merge_range_andplus3");

	RETURN;
}

static int test_range_and_otherwise(void)
{
	addr pos, array;

	vector4_heap(&array, 1);
	parse_type_string(&pos, "(and)");
	SetArrayA4(array, 0, pos);
	range_and_otherwise(NULL, &pos, array, LISPDECL_INTEGER);
	test(pos == Nil, "range_and_otherwise1");

	vector4_heap(&array, 1);
	parse_type_string(&pos, "(or)");
	SetArrayA4(array, 0, pos);
	range_and_otherwise(NULL, &pos, array, LISPDECL_INTEGER);
	test(pos == Nil, "range_and_otherwise2");

	vector4_heap(&array, 2);
	parse_type_string(&pos, "(integer 10 30)");
	SetArrayA4(array, 0, pos);
	parse_type_string(&pos, "(integer 20 40)");
	SetArrayA4(array, 1, pos);
	range_and_otherwise(NULL, &pos, array, LISPDECL_INTEGER);
	test(singlep(pos), "range_and_otherwise3");
	GetCar(pos, &pos);
	parse_type_string(&array, "(integer 20 30)");
	test(equal_real(pos, array), "range_and_otherwise4");

	RETURN;
}

static int test_merge_range_and(void)
{
	addr left, right;

	parse_type_string(&left, "(and)");
	merge_range_and(NULL, &left, left, LISPDECL_INTEGER);
	test(left == T, "merge_range_and1");

	parse_type_string(&left, "(and (integer 10 20))");
	merge_range_and(NULL, &left, left, LISPDECL_INTEGER);
	test(singlep(left), "merge_range_and2");
	GetCar(left, &left);
	parse_type_string(&right, "(integer 10 20)");
	test(equal_real(left, right), "merge_range_and3");

	parse_type_string(&left, "(and (integer 10 30) (integer 20 40))");
	merge_range_and(NULL, &left, left, LISPDECL_INTEGER);
	test(singlep(left), "merge_range_and4");
	GetCar(left, &left);
	parse_type_string(&right, "(integer 20 30)");
	test(equal_real(left, right), "merge_range_and5");

	RETURN;
}


/*
 *  merge-range-or
 */
static int extpaircall_right1(LocalRoot local, addr *ret, addr left, addr right)
{
	if (GetType(left) == LISPTYPE_FIXNUM && GetType(right) == LISPTYPE_FIXNUM) {
		if (fixnumcompare(left, right) < 0) {
			*ret = Nil;
			return 1;
		}
	}
	return 0;
}

static int test_extpaircall_right(void)
{
	int check;
	addr pos;

	list_heap(&pos, fixnum_heapr(10), fixnum_heapr(20), fixnum_heapr(30), NULL);
	check = extpaircall_right(NULL, extpaircall_right1, &pos, fixnum_heapr(15), pos);
	test(check, "extpaircall_right1");
	test(pos == Nil, "extpaircall_right2");

	list_heap(&pos, fixnum_heapr(10), fixnum_heapr(20), fixnum_heapr(30), NULL);
	check = extpaircall_right(NULL, extpaircall_right1, &pos, fixnum_heapr(40), pos);
	test(! check, "extpaircall_right3");

	RETURN;
}

static int test_pushlist(void)
{
	addr cons, result, check;

	result = cons = Nil;
	pushlist(NULL, &result, cons, result);
	test(result == Nil, "pushlist1");

	result = Nil;
	list_heap(&cons, fixnum_heapr(10), fixnum_heapr(20), NULL);
	pushlist(NULL, &result, cons, result);
	check = result;
	test(GetType(check) == LISPTYPE_CONS, "pushlist2");
	GetCons(check, &cons, &check);
	test(RefFixnum(cons) == 20, "pushlist3");
	GetCons(check, &cons, &check);
	test(RefFixnum(cons) == 10, "pushlist4");
	test(check == Nil, "pushlist5");

	list_heap(&cons, fixnum_heapr(30), fixnum_heapr(40), NULL);
	pushlist(NULL, &result, cons, result);
	check = result;
	GetCons(check, &cons, &check);
	test(RefFixnum(cons) == 40, "pushlist6");
	GetCons(check, &cons, &check);
	test(RefFixnum(cons) == 30, "pushlist7");
	GetCons(check, &cons, &check);
	test(RefFixnum(cons) == 20, "pushlist8");
	GetCons(check, &cons, &check);
	test(RefFixnum(cons) == 10, "pushlist9");
	test(check == Nil, "pushlist10");

	RETURN;
}

static int test_extpaircall_left(void)
{
	int check;
	addr pos, one;

	list_heap(&pos, fixnum_heapr(10), fixnum_heapr(30), fixnum_heapr(20), NULL);
	check = extpaircall_left(NULL, extpaircall_right1, &pos, pos);
	test(check, "extpaircall_left1");
	GetCons(pos, &one, &pos);
	test(one == Nil, "extpaircall_left2");
	GetCons(pos, &one, &pos);
	test(RefFixnum(one) == 30, "extpaircall_left3");
	GetCons(pos, &one, &pos);
	test(RefFixnum(one) == 20, "extpaircall_left4");
	test(pos == Nil, "extpaircall_left5");

	list_heap(&pos, fixnum_heapr(30), fixnum_heapr(30), fixnum_heapr(30), NULL);
	check = extpaircall_left(NULL, extpaircall_right1, &pos, pos);
	test(! check, "extpaircall_left6");

	RETURN;
}

static int test_extpaircall(void)
{
	int check;
	addr pos, one;

	list_heap(&pos, fixnum_heapr(10), fixnum_heapr(30), fixnum_heapr(20), NULL);
	check = 0;
	extpaircall(NULL, extpaircall_right1, &pos, &check);
	test(check, "extpaircall1");
	GetCons(pos, &one, &pos);
	test(one == Nil, "extpaircall2");
	GetCons(pos, &one, &pos);
	test(RefFixnum(one) == 30, "extpaircall3");
	GetCons(pos, &one, &pos);
	test(one == Nil, "extpaircall4");
	test(pos == Nil, "extpaircall5");

	list_heap(&pos, fixnum_heapr(30), fixnum_heapr(30), fixnum_heapr(30), NULL);
	check = 0;
	extpaircall(NULL, extpaircall_right1, &pos, &check);
	test(! check, "extpaircall6");

	RETURN;
}

static int test_range_or_aster(void)
{
	int check;
	addr pos;

	parse_type_string(&pos, "integer");
	check = range_or_aster(NULL, &pos, Nil, pos);
	test(check == -1, "range_or_aster1");
	test(pos == Nil, "range_or_aster2");

	check = range_or_aster(NULL, &pos, Nil, Nil);
	test(check == 0, "range_or_aster3");

	RETURN;
}

static int range_delete_p(int check, addr pos)
{
	return check == -1 && pos == Nil;
}

static int test_range_or_left_left(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_left_left1");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 30 *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(check == 0, "range_or_left_left2");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 20 *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_left_left3");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer 20 *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_left_left4");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer (20) *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(check == 0, "range_or_left_left5");

	parse_type_string(&left, "(integer (20) 40)");
	parse_type_string(&right, "(integer (20) *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_left_left6");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer 10 *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_left_left7");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer 20 *)");
	check = range_or_left_left(NULL, &left, left, right);
	test(check == 0, "range_or_left_left8");

	RETURN;
}

static int test_range_or_right_right(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 50)");
	check = range_or_right_right(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_right_right1");

	parse_type_string(&left, "(integer * 40)");
	parse_type_string(&right, "(integer * 50)");
	check = range_or_right_right(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_right_right2");

	parse_type_string(&left, "(integer * 40)");
	parse_type_string(&right, "(integer * 30)");
	check = range_or_right_right(NULL, &left, left, right);
	test(check == 0, "range_or_right_right3");

	parse_type_string(&left, "(integer * 40)");
	parse_type_string(&right, "(integer * 40)");
	check = range_or_right_right(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_right_right4");

	parse_type_string(&left, "(integer * (40))");
	parse_type_string(&right, "(integer * 40)");
	check = range_or_right_right(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_right_right5");

	parse_type_string(&left, "(integer * 40)");
	parse_type_string(&right, "(integer * (40))");
	check = range_or_right_right(NULL, &left, left, right);
	test(check == 0, "range_or_right_right6");

	parse_type_string(&left, "(integer * (40))");
	parse_type_string(&right, "(integer * (40))");
	check = range_or_right_right(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_right_right7");

	RETURN;
}

static int integer_aster_p(int check, addr pos)
{
	return check && RefLispDecl(pos) == LISPDECL_INTEGER && range_asterisk_p(pos);
}

static int test_range_or_left_right(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer * 20)");
	check = range_or_left_right(NULL, &left, left, right);
	test(integer_aster_p(check, left), "range_or_left_right1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer * 10)");
	check = range_or_left_right(NULL, &left, left, right);
	test(! integer_aster_p(check, left), "range_or_left_right2");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer * 10)");
	check = range_or_left_right(NULL, &left, left, right);
	test(integer_aster_p(check, left), "range_or_left_right3");

	parse_type_string(&left, "(integer (10) *)");
	parse_type_string(&right, "(integer * 10)");
	check = range_or_left_right(NULL, &left, left, right);
	test(integer_aster_p(check, left), "range_or_left_right4");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer * (10))");
	check = range_or_left_right(NULL, &left, left, right);
	test(integer_aster_p(check, left), "range_or_left_right5");

	parse_type_string(&left, "(integer (10) *)");
	parse_type_string(&right, "(integer * (10))");
	check = range_or_left_right(NULL, &left, left, right);
	test(! integer_aster_p(check, left), "range_or_left_right6");

	RETURN;
}

static int test_range_or_range_left(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 20 *)");
	check = range_or_range_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 *)");
	test(check && equal_real(left, right), "range_or_range_left1");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 5 *)");
	check = range_or_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_left2");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 40 *)");
	check = range_or_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_left3");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 30 *)");
	check = range_or_range_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 *)");
	test(check && equal_real(left, right), "range_or_range_left4");

	parse_type_string(&left, "(integer 10 (30))");
	parse_type_string(&right, "(integer 30 *)");
	check = range_or_range_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 *)");
	test(check && equal_real(left, right), "range_or_range_left5");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer (30) *)");
	check = range_or_range_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 *)");
	test(check && equal_real(left, right), "range_or_range_left6");

	parse_type_string(&left, "(integer 10 (30))");
	parse_type_string(&right, "(integer (30) *)");
	check = range_or_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_left7");

	RETURN;
}

static int test_range_or_range_right(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer * 20)");
	check = range_or_range_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer * 30)");
	test(check && equal_real(left, right), "range_or_range_right1");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer * 5)");
	check = range_or_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_right2");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer * 40)");
	check = range_or_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_right3");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer * 10)");
	check = range_or_range_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer * 30)");
	test(check && equal_real(left, right), "range_or_range_right4");

	parse_type_string(&left, "(integer (10) 30)");
	parse_type_string(&right, "(integer * 10)");
	check = range_or_range_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer * 30)");
	test(check && equal_real(left, right), "range_or_range_right5");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer * (10))");
	check = range_or_range_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer * 30)");
	test(check && equal_real(left, right), "range_or_range_right6");

	parse_type_string(&left, "(integer (10) 30)");
	parse_type_string(&right, "(integer * (10))");
	check = range_or_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_right7");

	RETURN;
}

static int test_range_or_range_range_in(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 20 30)");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in1");

	parse_type_string(&left, "(integer 0 30)");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_in2");

	parse_type_string(&left, "(integer 20 50)");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_in3");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in4");

	parse_type_string(&left, "(integer (10) 30)");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in5");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer (10) 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_in6");

	parse_type_string(&left, "(integer (10) 30)");
	parse_type_string(&right, "(integer (10) 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in7");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in8");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer 10 40)");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in9");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 (40))");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_in10");

	parse_type_string(&left, "(integer 20 (40))");
	parse_type_string(&right, "(integer 10 (40))");
	check = range_or_range_range_in(NULL, &left, left, right);
	test(range_delete_p(check, left), "range_or_range_range_in11");

	RETURN;
}

static int test_range_or_range_range_left(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 20 40)");
	check = range_or_range_range_left(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 40)");
	test(check && equal_real(left, right), "range_or_range_range_left1");

	parse_type_string(&left, "(integer 10 *)");
	parse_type_string(&right, "(integer 20 40)");
	check = range_or_range_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_left2");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer * 40)");
	check = range_or_range_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_left3");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 35 40)");
	check = range_or_range_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_left4");

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 20 25)");
	check = range_or_range_range_left(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_left5");

	RETURN;
}

static int test_range_or_range_range_right(void)
{
	int check;
	addr left, right;

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 30)");
	check = range_or_range_range_right(NULL, &left, left, right);
	parse_type_string(&right, "(integer 10 40)");
	test(check && equal_real(left, right), "range_or_range_range_right1");

	parse_type_string(&left, "(integer 20 *)");
	parse_type_string(&right, "(integer 10 30)");
	check = range_or_range_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_right2");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer * 30)");
	check = range_or_range_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_right3");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 25 30)");
	check = range_or_range_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_right4");

	parse_type_string(&left, "(integer 20 40)");
	parse_type_string(&right, "(integer 10 50)");
	check = range_or_range_range_right(NULL, &left, left, right);
	test(check == 0, "range_or_range_range_right5");

	RETURN;
}

static int test_merge_range_orplus(void)
{
	addr left, right;

	parse_type_string(&left, "(integer 10 30)");
	parse_type_string(&right, "(integer 20 50)");
	list_heap(&left, left, right, NULL);
	parse_type_string(&right, "(integer 40 100)");
	list_heap(&right, right, NULL);
	merge_range_orplus(NULL, &left, left, right);
	test(singlep(left), "merge_range_orplus1");
	GetCar(left, &left);
	parse_type_string(&right, "(integer 10 100)");
	test(equal_real(left, right), "merge_range_orplus2");

	RETURN;
}

static int test_range_or_otherwise(void)
{
	addr pos, array;

	vector4_heap(&array, 1);
	parse_type_string(&pos, "(or)");
	SetArrayA4(array, 0, pos);
	range_or_otherwise(NULL, &pos, array, LISPDECL_INTEGER);
	test(pos == Nil, "range_or_otherwise1");

	vector4_heap(&array, 2);
	parse_type_string(&pos, "(and)");
	SetArrayA4(array, 0, pos);
	parse_type_string(&pos, "(integer 10 20)");
	SetArrayA4(array, 1, pos);
	range_or_otherwise(NULL, &pos, array, LISPDECL_INTEGER);
	test(pos == T, "range_or_otherwise2");

	vector4_heap(&array, 2);
	parse_type_string(&pos, "(integer 10 30)");
	SetArrayA4(array, 0, pos);
	parse_type_string(&pos, "(integer 20 40)");
	SetArrayA4(array, 1, pos);
	range_or_otherwise(NULL, &pos, array, LISPDECL_INTEGER);
	test(singlep(pos), "range_or_otherwise3");
	GetCar(pos, &pos);
	parse_type_string(&array, "(integer 10 40)");
	test(equal_real(pos, array), "range_or_otherwise4");

	RETURN;
}

static int test_merge_range_or(void)
{
	addr pos, check;

	parse_type_string(&pos, "(or)");
	merge_range_or(NULL, &pos, pos, LISPDECL_INTEGER);
	test(pos == Nil, "merge_range_or1");

	parse_type_string(&pos, "(or integer)");
	GetArrayType(pos, 0, &check);
	GetArrayA4(check, 0, &check);
	merge_range_or(NULL, &pos, pos, LISPDECL_INTEGER);
	test(singlep(pos), "merge_range_or2");
	GetCar(pos, &pos);
	test(pos == check, "merge_range_or3");

	parse_type_string(&pos, "(or (integer 10 30) (integer 20 40))");
	merge_range_or(NULL, &pos, pos, LISPDECL_INTEGER);
	test(singlep(pos), "merge_range_or4");
	GetCar(pos, &pos);
	parse_type_string(&check, "(integer 10 40)");
	test(equal_real(pos, check), "merge_range_or5");

	RETURN;
}

static int test_merge_range_type(void)
{
	addr pos, check;

	parse_type_string(&pos, "(and (integer 10 30) (integer 20 40))");
	merge_range_type(NULL, &pos, pos, LISPDECL_INTEGER);
	test(singlep(pos), "merge_range_type1");
	GetCar(pos, &pos);
	parse_type_string(&check, "(integer 20 30)");
	test(equal_real(pos, check), "merge_range_type2");

	parse_type_string(&pos, "(or (integer 10 30) (integer 20 40))");
	merge_range_type(NULL, &pos, pos, LISPDECL_INTEGER);
	test(singlep(pos), "merge_range_type3");
	GetCar(pos, &pos);
	parse_type_string(&check, "(integer 10 40)");
	test(equal_real(pos, check), "merge_range_type4");

	parse_type_string(&pos, "(integer 10 30)");
	merge_range_type(NULL, &pos, pos, LISPDECL_INTEGER);
	test(singlep(pos), "merge_range_type5");
	GetCar(pos, &pos);
	parse_type_string(&check, "(integer 10 30)");
	test(equal_real(pos, check), "merge_range_type6");

	RETURN;
}

static int test_type_or_cons(void)
{
	addr pos, check;

	parse_type_string(&check, "integer");
	list_heap(&pos, check, NULL);
	type_or_cons(NULL, &pos, pos);
	test(RefLispDecl(pos) == LISPDECL_OR, "type_or_cons1");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 1, "type_or_cons2");
	GetArrayA4(pos, 0, &pos);
	test(pos == check, "type_or_cons3");

	RETURN;
}

static int test_make_merge_range(void)
{
	addr left, right;

	parse_type_string(&left, "(or)");
	make_merge_range(NULL, &left, left, LISPDECL_INTEGER);
	test(left == Nil, "make_merge_range1");

	parse_type_string(&left, "(and)");
	make_merge_range(NULL, &left, left, LISPDECL_INTEGER);
	parse_type_string(&right, "integer");
	test(equal_real(left, right), "make_merge_range2");

	parse_type_string(&left, "integer");
	make_merge_range(NULL, &left, left, LISPDECL_INTEGER);
	parse_type_string(&right, "integer");
	test(equal_real(left, right), "make_merge_range3");

	RETURN;
}

static int test_merge_range(void)
{
	addr pos;

	parse_type_string(&pos, "(or (integer 10 20) (integer 20 30))");
	merge_range(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_INTEGER, "merge_range1");

	parse_type_string(&pos, "(or (integer 10 20) (integer 30 40))");
	merge_range(NULL, &pos, pos, LISPDECL_INTEGER);
	test(RefLispDecl(pos) == LISPDECL_OR, "merge_range2");

	type_nil_heap(&pos);
	merge_range(NULL, &pos, pos, LISPDECL_INTEGER);
	test(pos == Nil, "merge_range3");

	RETURN;
}


/*
 *  real_extract
 */
static int test_real_filter_range_list(void)
{
	addr pos;
	size_t size;
	LocalRoot local;
	LocalStack stack;

	local = Local_Thread;
	push_local(local, &stack);
	parse_type_string(&pos, "rational");
	size = real_filter_range_list(local, &pos, pos);
	test(size == 2, "real_filter_range_list1");
	GetCdr(pos, &pos);
	test(pos != Nil, "real_filter_range_list2");
	GetCdr(pos, &pos);
	test(pos == Nil, "real_filter_range_list3");

	parse_type_string(&pos, "string");
	size = real_filter_range_list(local, &pos, pos);
	test(size == 0, "real_filter_range_list4");
	test(pos == Nil, "real_filter_range_list5");

	rollback_local(local, stack);

	RETURN;
}

static int test_real_reject(void)
{
	addr pos, check;

	parse_type_string(&pos, "symbol");
	real_reject(NULL, &pos, pos);
	test(RefLispDecl(pos) == LISPDECL_AND, "real_reject1");
	GetArrayType(pos, 0, &pos);
	test(lenarrayr(pos) == 2, "real_reject2");
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_REAL, "real_reject3");
	test(RefNotDecl(check), "real_reject4");
	GetArrayA4(pos, 1, &check);
	test(RefLispDecl(check) == LISPDECL_SYMBOL, "real_reject5");

	RETURN;
}

static int test_copy_cons_to_vector4_alloc(void)
{
	addr pos, check;

	list_heap(&pos, T, fixnum_heapr(10), fixnum_heapr(20), NULL);
	copy_cons_to_vector4_alloc(NULL, &pos, pos, 2);
	test(lenarrayr(pos) == 2, "copy_cons_to_vector4_alloc1");
	GetArrayA4(pos, 0, &check);
	test(check == T, "copy_cons_to_vector4_alloc2");
	GetArrayA4(pos, 1, &check);
	test(RefFixnum(check) == 10, "copy_cons_to_vector4_alloc3");

	RETURN;
}

static int test_make_real_filter(void)
{
	addr pos, check;

	parse_type_string(&pos, "symbol");
	make_real_filter(NULL, &check, pos);
	test(pos == check, "make_real_filter1");

	parse_type_string(&pos, "rational");
	make_real_filter(NULL, &pos, pos);
	test(RefLispDecl(pos) == LISPDECL_OR, "make_real_filter2");
	GetArrayType(pos, 0, &pos);
	GetArrayA4(pos, 0, &check);
	test(RefLispDecl(check) == LISPDECL_AND, "make_real_filter3");
	GetArrayA4(pos, 1, &check);
	test(RefLispDecl(check) == LISPDECL_INTEGER, "make_real_filter4");
	GetArrayA4(pos, 2, &check);
	test(RefLispDecl(check) == LISPDECL_RATIONAL, "make_real_filter5");

	RETURN;
}

static int test_real_extract(void)
{
	addr pos;

	parse_type_string(&pos, "(integer 10 20)");
	real_extract(NULL, &pos, pos);
	test(GetType(pos) == LISPTYPE_TYPE, "real_extract1");

	RETURN;
}


/*
 *  main
 */
static int testbreak_type_number(void)
{
	/* real_filter */
	TestBreak(test_type_range_left);
	TestBreak(test_type_range_right);
	TestBreak(test_type_range_not);
	TestBreak(test_real_filter_not_range);
	TestBreak(test_real_filter_not);
	TestBreak(test_real_filter_normal);
	TestBreak(test_real_filter_type);
	TestBreak(test_real_filter_and);
	TestBreak(test_real_filter_or);
	TestBreak(test_real_filter);
	/* merge_range */
	TestBreak(test_merge_range_cons);
	/* merge-range-and */
	TestBreak(test_make_range_left_right);
	TestBreak(test_make_range_left_aster);
	TestBreak(test_make_range_aster_right);
	TestBreak(test_range_and_left_left);
	TestBreak(test_range_and_right_left);
	TestBreak(test_range_and_between_left);
	TestBreak(test_range_and_left);
	TestBreak(test_range_and_left_right);
	TestBreak(test_range_and_right_right);
	TestBreak(test_range_and_between_right);
	TestBreak(test_range_and_right);
	TestBreak(test_range_and_between_between);
	TestBreak(test_range_and_between);
	TestBreak(test_range_and);
	TestBreak(test_map_range_and);
	TestBreak(test_merge_range_andplus);
	TestBreak(test_range_and_otherwise);
	TestBreak(test_merge_range_and);
	/* merge-range-or */
	TestBreak(test_extpaircall_right);
	TestBreak(test_pushlist);
	TestBreak(test_extpaircall_left);
	TestBreak(test_extpaircall);
	TestBreak(test_range_or_aster);
	TestBreak(test_range_or_left_left);
	TestBreak(test_range_or_right_right);
	TestBreak(test_range_or_left_right);
	TestBreak(test_range_or_range_left);
	TestBreak(test_range_or_range_right);
	TestBreak(test_range_or_range_range_in);
	TestBreak(test_range_or_range_range_left);
	TestBreak(test_range_or_range_range_right);
	TestBreak(test_merge_range_orplus);
	TestBreak(test_range_or_otherwise);
	TestBreak(test_merge_range_or);
	TestBreak(test_merge_range_type);
	TestBreak(test_type_or_cons);
	TestBreak(test_make_merge_range);
	TestBreak(test_merge_range);
	/* real_extract */
	TestBreak(test_real_filter_range_list);
	TestBreak(test_real_reject);
	TestBreak(test_copy_cons_to_vector4_alloc);
	TestBreak(test_make_real_filter);
	TestBreak(test_real_extract);

	return 0;
}

int test_type_number(void)
{
	int result;
	lispcode code;
	Execute ptr;

	TITLE;

	freelisp();
	alloclisp(0, 0);
	lisp_info_enable = 1;
	ptr = Execute_Thread;
	begin_code(ptr, &code);
	if (code_run_p(code)) {
		build_lisproot(ptr);
		build_constant();
		build_object();
		build_character();
		build_package();
		build_stream();
		build_symbol();
		build_clos(ptr);
		build_condition(ptr);
		build_type();
		build_calltype();
		build_syscall();
		build_common();
		build_readtable();
		lisp_init = 1;
		result = testbreak_type_number();
	}
	end_code(ptr);
	freelisp();
	TestCheck(code_error_p(code));
	lisp_info_enable = 1;

	return result;
}
