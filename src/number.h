#ifndef __NUMBER_HEADER__
#define __NUMBER_HEADER__

#include "lisp.h"
#include "local.h"
#include "real.h"

enum Boole_Index {
	Boole_1,
	Boole_2,
	Boole_And,
	Boole_AndC1,
	Boole_AndC2,
	Boole_C1,
	Boole_C2,
	Boole_Clr,
	Boole_Eqv,
	Boole_Ior,
	Boole_Nand,
	Boole_Nor,
	Boole_Orc1,
	Boole_Orc2,
	Boole_Set,
	Boole_Xor,
	Boole_Size
};

int numberp(addr pos);

int number_result_alloc(LocalRoot local, addr pos, addr *ret);
void number_throw_alloc(LocalRoot local, addr pos, addr *ret);
void number_throw_local(LocalRoot local, addr pos, addr *ret);
void number_throw_heap(addr pos, addr *ret);

#define plusp_number plusp_real
#define minusp_number minusp_real
int zerop_number(addr left);
int equal_number(LocalRoot local, addr left, addr right);
#define not_equal_number(m,a,b) (! equal_number((m),(a),(b)))
#define compare_number compare_real
#define less_number less_real
#define greater_number greater_real
#define less_equal_number less_equal_real
#define greater_equal_number greater_equal_real

void oneplus_number_heap(LocalRoot local, addr value, addr *ret);
void oneminus_number_heap(LocalRoot local, addr value, addr *ret);
void sign_reverse_number_common(addr left, addr *ret);
void sign_reverse_number_local(LocalRoot local, addr left, addr *ret);

void plus_number_heap(LocalRoot local, addr left, addr right, addr *ret);
void minus_number_heap(LocalRoot local, addr left, addr right, addr *ret);
void multi_number_heap(LocalRoot local, addr left, addr right, addr *ret);
void inverse_number_heap(LocalRoot local, addr left, addr *ret);
void div_number_heap(LocalRoot local, addr left, addr right, addr *ret);
void mod_number_heap(LocalRoot local, addr left, addr right, addr *ret);
void rem_number_heap(LocalRoot local, addr left, addr right, addr *ret);

void abs_number_heap(LocalRoot local, addr left, addr *ret);
void random_number_heap(addr limit, addr state, addr *ret);

#endif

