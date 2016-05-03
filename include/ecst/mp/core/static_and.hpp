// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core/aliases.hpp>

ECST_MP_NAMESPACE
{
    namespace impl
    {
        template <typename TP0, typename TP1>
        constexpr auto static_and_impl(TP0 p0, TP1 p1) noexcept
        {
            return static_if(bool_v<p0>)
                .then([](auto xp1)
                    {
                        return bool_v<xp1>;
                    })
                .else_([](auto)
                    {
                        return bool_v<false>;
                    })(p1);
        }
    }

    template <typename TP0, typename TP1>
    constexpr auto static_and(TP0 p0, TP1 p1) noexcept
    {
        return decltype(impl::static_and_impl(p0, p1)){};
    }
}
ECST_MP_NAMESPACE_END
