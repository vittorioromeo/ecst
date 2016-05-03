// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/mp/adjacency_list/types.hpp>

ECST_MP_NAMESPACE
{
    namespace impl
    {
#if defined(VRM_CORE_COMPILER_GCC)
        template <typename T>
        using is_adjacency_list_impl =
            is_specialization_of<mp::adjacency_list::impl::type_adjacency_list,
                std::decay_t<T>>;
#else
        template <typename T>
        using is_adjacency_list_impl = bool_<true>;
#endif
    }

    template <typename T>
    constexpr auto is_adjacency_list(T)
    {
        return bool_v<impl::is_adjacency_list_impl<T>{}>;
    }
}
ECST_MP_NAMESPACE_END
