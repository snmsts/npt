#ifndef __ARRAY_OBJECT_HEADER__
#define __ARRAY_OBJECT_HEADER__

#include "array_typedef.h"
#include "local.h"
#include "typedef.h"
#include "memory.h"

/* control */
_g void array_settype(addr pos);
_g void array_element_size(addr pos);
_g int array_equal_type(struct array_struct *a, enum ARRAY_TYPE type, unsigned size);
_g void array_element_type(addr pos, addr *ret);
_g size_t array_vector_length(addr pos, int fill);
_g void array_rowlength(addr pos, size_t *ret);
_g int array_dimension_equal(addr left, addr right);

/*  array object */
_g void array_setsize_heap(addr pos, addr value);
_g void array_allocate_bit(LocalRoot local, addr pos, struct array_struct *str);
_g void array_allocate_size(LocalRoot local, addr pos, struct array_struct *str);
_g void array_allocate(LocalRoot local, addr pos, struct array_struct *str);
_g void array_memory_make(LocalRoot local, addr pos,
		addr adjustable, addr fillpointer, addr displaced, addr offset);
_g void array_initial_make(LocalRoot local, addr pos, addr initial, addr contents);
_g void make_array_common(addr *ret, addr dimension,
		addr type, addr initial, addr contents,
		addr adjustable, addr fillpointer, addr displaced, addr offset);
_g void array_contents_heap(addr *ret, addr rank, addr contents);
_g void array_character_alloc(LocalRoot local, addr pos);
_g void array_build_alloc(LocalRoot local, addr pos);
_g void array_build_heap(addr pos);

/* array */
_g int array_get_t(addr pos, size_t index, addr *ret);
_g int array_get_bit(addr pos, size_t index, int *ret);
_g int array_get_unicode(addr pos, size_t index, unicode *ret);
_g void array_get(LocalRoot local, addr pos, size_t index, addr *ret);
_g int array_set_bit(LocalRoot local, addr pos, size_t index, int value);
_g int array_set_character(LocalRoot local, addr pos, size_t index, unicode value);
_g int array_set_signed8(LocalRoot local, addr pos, size_t index, int8_t value);
_g int array_set_signed16(LocalRoot local, addr pos, size_t index, int16_t value);
_g int array_set_signed32(LocalRoot local, addr pos, size_t index, int32_t value);
#ifdef LISP_64BIT
_g int array_set_signed64(LocalRoot local, addr pos, size_t index, int64_t value);
#endif
_g int array_set_unsigned8(LocalRoot local, addr pos, size_t index, uint8_t value);
_g int array_set_unsigned16(LocalRoot local, addr pos, size_t index, uint16_t value);
_g int array_set_unsigned32(LocalRoot local, addr pos, size_t index, uint32_t value);
#ifdef LISP_64BIT
_g int array_set_unsigned64(LocalRoot local, addr pos, size_t index, uint64_t value);
#endif
_g int array_set_single(LocalRoot local, addr pos, size_t index, single_float value);
_g int array_set_double(LocalRoot local, addr pos, size_t index, double_float value);
_g int array_set_long(LocalRoot local, addr pos, size_t index, long_float value);
_g void array_set(addr pos, size_t index, addr value);
_g void array_setget(addr p1, size_t s1, addr p2, size_t s2);
_g void array_get_inplace(addr pos, size_t index, struct array_value *value);
_g int array_set_inplace(addr pos, size_t index, const struct array_value *value);
_g void array_aref(LocalRoot local, addr pos, addr args, addr *ret);
_g void array_setf_aref(addr pos, addr args, addr value);
_g void array_aref_bit(LocalRoot local, addr pos, addr args, addr *ret);
_g void array_setf_aref_bit(addr pos, addr args, addr value);

_g void array_array_dimension(addr array, addr axis, addr *ret);
_g void array_array_dimensions(addr array, addr *ret);
_g void array_array_displacement(addr array, addr *displaced, addr *offset);
_g int array_array_in_bounds_p(addr array, addr rest);
_g void array_array_row_major_index(addr array, addr rest, addr *ret);
_g int array_fill_pointer(addr array, addr *ret);
_g int array_setf_fill_pointer(addr array, addr value);
_g int array_fill_pointer_start(addr array);
_g int array_fill_pointer_end(addr array);
_g int array_fill_pointer_set(addr array, size_t size);

_g void array_value_alloc(LocalRoot local, addr *ret, const struct array_value *str);
_g void array_value_local(LocalRoot local, addr *ret, const struct array_value *str);
_g void array_value_heap(addr *ret, const struct array_value *str);

#endif

