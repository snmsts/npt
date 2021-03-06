/*
 *  xorshift+
 *
 *  Xorshift RNGs
 *    George Marsaglia, The Florida State University,
 *    Journal of Statistical Software, 2003.
 *    http://www.jstatsoft.org/v08/i14/paper
 *
 *  Further scramblings of Marsaglia's xorshift generators
 *    Sebastiano Vigna, Universit`a degli Studi di Milano, Italy,
 *    arXiv:1404.0390v3 [cs.DS] 23 May 2016.
 *    https://arxiv.org/abs/1404.0390
 *    http://vigna.di.unimi.it/ftp/papers/xorshiftplus.pdf
 *
 *  Usage:
 *    struct random_state state;
 *    random_seed_string(&state, "Hello");
 *    printf("%lu\n", random_number_64bit(&state));
 *    printf("%lu\n", random_number_64bit(&state));
 *    printf("%lu\n", random_number_64bit(&state));
 */
#ifndef __RANDOM_HEADER__
#define __RANDOM_HEADER__

#include "define.h"
#include <stddef.h>
#include <stdint.h>

struct random_state {
	union {
		uint64_t u64[2];
		uint32_t u32[4];
	} seed;
};

/* random */
_g uint32_t random_number_32bit(struct random_state *state);
_g uint64_t random_number_64bit(struct random_state *state);
/* 0 ... value */
_g uint32_t random_equal_32bit(struct random_state *state, uint32_t value);
_g uint64_t random_equal_64bit(struct random_state *state, uint64_t value);
/* 0 ... value-1 */
_g uint32_t random_less_32bit(struct random_state *state, uint32_t value);
_g uint64_t random_less_64bit(struct random_state *state, uint64_t value);
/* seed */
_g void random_seed_buffer(struct random_state *state, const void *ptr, size_t size);
_g void random_seed_string(struct random_state *state, const char *word);
/* check */
_g int random_state_equal(struct random_state *a, struct random_state *b);

#endif

