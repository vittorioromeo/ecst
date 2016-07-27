// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    template <sz_t TAlignment>
    ECST_ALWAYS_INLINE constexpr auto ECST_CONST_FN aligned_ptr(
        const char* ptr) noexcept
    {
        const auto int_ptr = reinterpret_cast<uintptr_t>(ptr);
        const auto aligned_int_ptr = (int_ptr - 1u + TAlignment) & -TAlignment;
        return reinterpret_cast<char*>(aligned_int_ptr);
    }
}
ECST_NAMESPACE_END
