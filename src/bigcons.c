/*
 *  bigcons
 */
#include <math.h>
#include "bigcons.h"
#include "bigdata.h"
#include "character.h"
#include "condition.h"

_g void bigbuffer_local(LocalRoot local, addr *ret, bigtype value)
{
	addr pos;
	struct bigbuffer *ptr;

	Check(0xFFFF < sizeoft(struct bigbuffer), "size error");
	local_arraybody(local, &pos, LISPTYPE_SYSTEM, 1, sizeof(struct bigbuffer));
	ptr = StructBigbuffer(pos);
#ifdef LISP_DEBUG
	aamemory(ptr, sizeof(struct bigbuffer));
#endif
	ptr->count = 1;
	ptr->buffer[0] = value;
	*ret = pos;
}

_g void bigcons_local(LocalRoot local, addr *ret)
{
	addr cons, pos;

	bigbuffer_local(local, &cons, 0);
	local_smallsize(local, &pos, LISPTYPE_SYSTEM, 1, sizeoft(struct bigcons_struct));
	SetUser(pos, 0);
	SetCountBigcons(pos, 1);
	SetRootBigcons(pos, cons);
	*ret = pos;
}

_g void clear_bigcons(addr cons)
{
	addr child, next;
	struct bigbuffer *ptr;

	GetRootBigcons(cons, &child);
	GetNextBigbuffer(child, &next);
	if (next != Nil)
		StructBigbuffer(next)->count = 0;
	ptr = StructBigbuffer(child);
	ptr->buffer[0] = 0;
	ptr->count = 1;
	SetUser(cons, 0);
	SetCountBigcons(cons, 1);
}

static void carrynext(LocalRoot local,
		addr cons, addr root, size_t len, bigtype carry)
{
	addr next;
	struct bigbuffer *ptr;

	if (carry && root != Nil) {
		GetNextBigbuffer(root, &next);
		if (len < BIGCONS_SIZE) {
			ptr = StructBigbuffer(root);
			ptr->buffer[len] = carry;
			ptr->count++;
		}
		else if (next != Nil) {
			ptr = StructBigbuffer(next);
			ptr->count = 1;
			ptr->buffer[0] = carry;
			GetNextBigbuffer(next, &next);
			if (next != Nil)
				StructBigbuffer(next)->count = 0;
		}
		else {
			bigbuffer_local(local, &next, carry);
			SetNextBigbuffer(root, next);
		}
		IncCountBigcons(cons, 1);
	}
}

static void plus_bigcons(LocalRoot local, addr cons, bigtype carry)
{
	bigtype *buffer;
	addr root, prev, next;
	size_t i, len;
	struct bigbuffer *ptr;

	if (carry == 0) return;
	len = 0;
	GetRootBigcons(cons, &root);
	for (prev = root; root != Nil; root = next) {
		ptr = StructBigbuffer(root);
		len = ptr->count;
		if (len == 0) {
			carrynext(local, cons, root, 0, carry);
			return;
		}
		buffer = ptr->buffer;
		for (i = 0; i < len; i++) {
			plusnumber_bigdata(&buffer[i], &carry);
			if (carry == 0) return;
		}
		prev = root;
		GetNextBigbuffer(root, &next);
	}
	carrynext(local, cons, prev, len, carry);
}

static void multi_bigcons(LocalRoot local, addr cons, bigtype value)
{
	bigtype carry, *buffer;
	addr root, prev, next;
	size_t i, len;
	struct bigbuffer *ptr;

	carry = 0;
	len = 0;
	GetRootBigcons(cons, &root);
	for (prev = root; root != Nil; root = next) {
		ptr = StructBigbuffer(root);
		len = ptr->count;
		if (len == 0) {
			carrynext(local, cons, root, 0, carry);
			return;
		}
		buffer = ptr->buffer;
		for (i = 0; i < len; i++)
			multicarry_bigdata(&buffer[i], value, &carry);
		prev = root;
		GetNextBigbuffer(root, &next);
	}
	carrynext(local, cons, prev, len, carry);
}

_g void push_bigcons(LocalRoot local, addr cons, unsigned base, unsigned digit)
{
	addr root;
	size_t count;
	fixed *buffer;

	Check(! isBaseChar(base), "base error");
	Check(base <= digit, "digit error");
	Check(BIGNUM_FULL <= (size_t)digit, "fullsize error");

	SetUser(cons, 1);
	GetCountBigcons(cons, &count);
	if (count == 1) {
		GetRootBigcons(cons, &root);
		buffer = StructBigbuffer(root)->buffer;
		if (buffer[0] == 0) {
			buffer[0] = (bigtype)digit;
			return;
		}
	}
	multi_bigcons(local, cons, (bigtype)base);
	plus_bigcons(local, cons, (bigtype)digit);
}

static int getnumber(unsigned base, int c, unsigned *ret)
{
	if ('0' <= c && c <= '9') {
		c -= '0';
	}
	else if ('A' <= c && c <= 'Z') {
		c = (c - 'A') + 10;
	}
	else if ('a' <= c && c <= 'z') {
		c = (c - 'a') + 10;
	}
	else {
		return 1;
	}

	if (base <= (unsigned)c) {
		return 1;
	}
	*ret = (unsigned)c;

	return 0;
}

_g void setchar_bigcons(LocalRoot local, addr pos, unsigned base, const char *value)
{
	int c;
	unsigned ret;
	addr error_character;
	size_t i;

	clear_bigcons(pos);
	for (i = 0; ; i++) {
		c = value[i];
		if (c == '\0') break;
		if (getnumber(base, c, &ret)) {
			character_heap(&error_character, (unicode)c);
			fmte("Invalid digit character ~S.", error_character, NULL);
			return;
		}
		push_bigcons(local, pos, base, ret);
	}
}

_g void bigcons_char_local(LocalRoot local, addr *ret, unsigned base, const char *value)
{
	bigcons_local(local, ret);
	setchar_bigcons(local, *ret, base, value);
}

_g int bigcons_empty_p(addr pos)
{
	return GetUser(pos) == 0;
}

