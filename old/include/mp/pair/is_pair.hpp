// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/mp/pair/types.hpp>

ECST_MP_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        using is_pair_impl =
            is_specialization_of_t<std::decay_t<T>, mp::list::impl::type_list>;
    }

    template <typename T>
    constexpr auto is_pair(T)
    {
        return bool_v<impl::is_pair_impl<T>{}>;
    }
}
ECST_MP_NAMESPACE_END
