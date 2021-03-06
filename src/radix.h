#ifndef __RADIX_HEADER__
#define __RADIX_HEADER__

#include "local.h"
#include "typedef.h"

_g void english_integer(LocalRoot local, addr stream, addr pos, int cardinal);
_g void english_unit_heap(LocalRoot local, addr *ret, addr pos, int cardinal);
_g void english_unit_local(LocalRoot local, addr *ret, addr pos, int cardinal);
_g void roma_integer(addr stream, fixnum value, int subp);

#endif

