#ifndef __NUMBER_HEADER__
#define __NUMBER_HEADER__

#include "build.h"
#include "local.h"
#include "real.h"

_g int numberp(addr pos);

_g void number_result_local(LocalRoot local, addr pos, addr *ret);
_g void number_result_heap(LocalRoot local, addr pos, addr *ret);
_g void number_throw_alloc(LocalRoot local, addr pos, addr *ret);
_g void number_throw_local(LocalRoot local, addr pos, addr *ret);
_g void number_throw_heap(addr pos, addr *ret);
_g void number_copy_alloc(LocalRoot local, addr pos, addr *ret);
_g void number_copy_local(LocalRoot local, addr pos, addr *ret);
_g void number_copy_heap(addr pos, addr *ret);

#define plusp_number plusp_real
#define minusp_number minusp_real
_g int zerop_number(addr left);
_g int equal_number(LocalRoot local, addr left, addr right);
#define not_equal_number(m,a,b) (! equal_number((m),(a),(b)))
#define compare_number compare_real
#define less_number less_real
#define greater_number greater_real
#define less_equal_number less_equal_real
#define greater_equal_number greater_equal_real

_g void oneplus_number_common(LocalRoot local, addr value, addr *ret);
_g void oneminus_number_common(LocalRoot local, addr value, addr *ret);
_g void sign_reverse_number_common(addr left, addr *ret);
_g void sign_reverse_number_local(LocalRoot local, addr left, addr *ret);

_g void plus_number_heap(LocalRoot local, addr left, addr right, addr *ret);
_g void minus_number_heap(LocalRoot local, addr left, addr right, addr *ret);
_g void multi_number_heap(LocalRoot local, addr left, addr right, addr *ret);
_g void inverse_number_heap(LocalRoot local, addr left, addr *ret);
_g void div_number_heap(LocalRoot local, addr left, addr right, addr *ret);

_g void abs_number_common(addr left, addr *ret);
_g void signum_number_common(addr pos, addr *ret);
_g void sqrt_number_common(addr pos, addr *ret);

#endif

