// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>

ECST_MP_NAMESPACE
{
    template <typename T, typename T0>
    constexpr auto min(T0 a) noexcept
    {
        return std::integral_constant<T, a>{};
    }

    template <typename T, typename T0, typename T1>
    constexpr auto min(T0 a, T1 b) noexcept
    {
        return std::integral_constant<T, (a < b ? a : b)>{};
    }

    template <typename T, typename T0, typename T1, typename... Ts>
    constexpr auto min(T0 a, T1 b, Ts... xs) noexcept
    {
        return std::integral_constant<T,
            (mp::min<T>(a, mp::min<T>(b, xs...)))>{};
    }
}
ECST_MP_NAMESPACE_END
