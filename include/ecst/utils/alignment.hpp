// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    // Example:
    // Want to align 5 to 8.
    // Bit representation of 5: 00000101
    // Bit representation of 8: 00001000
    //
    // Operations:
    // `(int_ptr - 1u + TAlignment) & -TAlignment`
    // `(int_ptr - 1u + 8) & -8`
    // `(00000101 - 00000001 + 00001000) & -00001000`
    // `00001100 & 11111000` (two's complement)
    // `00001000` = 8

    // The two's complement of the alignment creates a mask where all bits to
    // the left of the alignment bits (including them) are set to 1.

    // This "switches off" all bits that would prevent the number to be a
    // multiple of 8.

    // More info:
    // http://stackoverflow.com/questions/1766535/bit-hack-round-off-to-multiple-of-8

    // "If you want to round up, first you have to add enough to "get past" the
    // next multiple of 8. Then the same masking step takes you back down to the
    // multiple of 8. The reason you choose 7 is that it's the only number
    // guaranteed to be "big enough" to get you from any number up past the next
    // multiple of 8 without going up an extra multiple if your original number
    // were already a multiple of 8."
    // - Carl Norum

    template <sz_t TAlignment>
    ECST_ALWAYS_INLINE constexpr auto ECST_CONST_FN aligned_ptr(
        const char* ptr) noexcept
    {
        // TODO: split into generic `unsigned int` "round to power of two"
        // function

        // TODO: assert `TAlignment` is a power of two

        const auto int_ptr = reinterpret_cast<uintptr_t>(ptr);
        const auto aligned_int_ptr = (int_ptr - 1u + TAlignment) & -TAlignment;
        return reinterpret_cast<char*>(aligned_int_ptr);
    }
}
ECST_NAMESPACE_END
