/*
 * SNES Station v0.23 -- compile-time Emotion Engine ABI contract.
 *
 * This file is a build-gate input, not a program translation unit.  It is
 * compiled with the frozen source-tree flags by tools/build_source_tree.py.
 * GCC 3.2.2 predates _Static_assert, so negative array bounds provide the
 * deliberately old-C-compatible assertions below.
 */

#define SNES_ABI_ASSERT(name, expression) \
    typedef char snes_abi_assert_##name[(expression) ? 1 : -1]

SNES_ABI_ASSERT(char_is_1, sizeof(char) == 1);
SNES_ABI_ASSERT(short_is_2, sizeof(short) == 2);
SNES_ABI_ASSERT(int_is_4, sizeof(int) == 4);
SNES_ABI_ASSERT(long_is_8, sizeof(long) == 8);
SNES_ABI_ASSERT(long_long_is_8, sizeof(long long) == 8);
SNES_ABI_ASSERT(pointer_is_4, sizeof(void *) == 4);
SNES_ABI_ASSERT(size_type_is_8, sizeof(__SIZE_TYPE__) == 8);
SNES_ABI_ASSERT(ptrdiff_type_is_8, sizeof(__PTRDIFF_TYPE__) == 8);
SNES_ABI_ASSERT(float_is_4, sizeof(float) == 4);
SNES_ABI_ASSERT(double_is_4, sizeof(double) == 4);
SNES_ABI_ASSERT(long_double_is_8, sizeof(long double) == 8);

#if !defined(__MIPSEL__)
#error "SNES Station source-tree contract requires little-endian MIPS"
#endif

#if !defined(__mips__) || !defined(__mips64)
#error "SNES Station source-tree contract requires the 64-bit-register MIPS ABI"
#endif
