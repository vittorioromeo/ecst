// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/core.hpp>

ECST_SETTINGS_NAMESPACE
{
    namespace impl
    {
        template <typename TSize>
        struct fixed_impl
        {
            using size = TSize;
        };

        template <typename TInitialSize>
        struct dynamic_impl
        {
            using initial_size = TInitialSize;
        };
    }

    template <sz_t TSize>
    constexpr impl::fixed_impl<mp::sz_t_<TSize>> fixed{};

    template <sz_t TInitialSize>
    constexpr impl::dynamic_impl<mp::sz_t_<TInitialSize>> dynamic{};
}
ECST_SETTINGS_NAMESPACE_END
