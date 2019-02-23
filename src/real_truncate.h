#ifndef __REAL_TRUNCATE_HEADER__
#define __REAL_TRUNCATE_HEADER__

#include "local.h"
#include "typedef.h"

void truncate_integer_common(LocalRoot local, addr *quot, addr *rem, addr left);
void ftruncate_integer_common(LocalRoot local, addr *quot, addr *rem, addr left);
void truncate_common(LocalRoot local, addr *quot, addr *rem, addr left, addr right);
void ftruncate_common(LocalRoot local, addr *quot, addr *rem, addr left, addr right);

#endif
