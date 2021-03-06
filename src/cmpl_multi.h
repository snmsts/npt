#ifndef __CMPL_MULTI_HEADER__
#define __CMPL_MULTI_HEADER__

#include "local.h"
#include "typedef.h"

_g void multi_rational_complex_common(LocalRoot local, addr left, addr right, addr *ret);
_g void multi_fc_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void multi_bc_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void multi_rc_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void multi_sc_number_common(addr left, addr right, addr *ret);
_g void multi_dc_number_common(addr left, addr right, addr *ret);
_g void multi_lc_number_common(addr left, addr right, addr *ret);
_g void multi_cc_number_common(LocalRoot local, addr left, addr right, addr *ret);
#define multi_cf_number_common(m,a,b,r) multi_fc_number_common((m),(b),(a),(r))
#define multi_cb_number_common(m,a,b,r) multi_bc_number_common((m),(b),(a),(r))
#define multi_cr_number_common(m,a,b,r) multi_rc_number_common((m),(b),(a),(r))
#define multi_cs_number_common(a,b,r) multi_sc_number_common((b),(a),(r))
#define multi_cd_number_common(a,b,r) multi_dc_number_common((b),(a),(r))
#define multi_cl_number_common(a,b,r) multi_lc_number_common((b),(a),(r))

_g void inverse_complex_common(LocalRoot local, addr pos, addr *ret);

_g void div_rational_complex_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_complex_rational_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_fc_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_cf_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_bc_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_cb_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_rc_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_cr_number_common(LocalRoot local, addr left, addr right, addr *ret);
_g void div_sc_number_common(addr left, addr right, addr *ret);
_g void div_cs_number_common(addr left, addr right, addr *ret);
_g void div_dc_number_common(addr left, addr right, addr *ret);
_g void div_cd_number_common(addr left, addr right, addr *ret);
_g void div_lc_number_common(addr left, addr right, addr *ret);
_g void div_cl_number_common(addr left, addr right, addr *ret);
_g void div_cc_number_common(LocalRoot local, addr left, addr right, addr *ret);

#endif

