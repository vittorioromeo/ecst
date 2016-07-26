// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./dependencies.hpp"

ECST_NAMESPACE
{
    /// @brief Round up `x` to the nearest multiple of `multiple`.
    template <typename T0, typename T1>
    constexpr auto multiple_round_up(T0 x, T1 multiple) noexcept
    {
        ECST_S_ASSERT(std::is_arithmetic<T0>{});
        ECST_S_ASSERT(std::is_arithmetic<T1>{});

        VRM_CORE_CONSTEXPR_ASSERT(multiple != 0);
        return ((x + multiple - 1) / multiple) * multiple;
    }
}
ECST_NAMESPACE_END
