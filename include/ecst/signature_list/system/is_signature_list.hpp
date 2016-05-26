// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        auto valid_impl(T&& x)
        {
            return mp::bh::all_of(x, [](auto sx)
                {
                    return signature::system::valid(mp::unwrapped(sx));
                });
        }
    }

    /// @brief Returns `true` if the passed argument is a system signature list.
    template <typename T>
    constexpr auto valid(T && x)
    {
        return decltype(impl::valid_impl(FWD(x))){};
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
