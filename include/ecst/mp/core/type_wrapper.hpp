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
        template <typename T>
        struct type_wrapper
        {
            using type = T;
        };
    }

    template <typename T>
    using type_ = impl::type_wrapper<T>;

    template <typename T>
    constexpr bh::type<T> type_v{};

    template <typename T>
    using unwrap = typename T::type;
}
ECST_MP_NAMESPACE_END
