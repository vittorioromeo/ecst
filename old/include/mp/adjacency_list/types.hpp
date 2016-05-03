// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/map.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        // A `type_adjacency_list` is a `mp::map`.
        template <typename... Ts>
        using type_adjacency_list = ecst::mp::map::impl::type_map<Ts...>;

        // An empty adjacency list is an empty `type_adjacency_list`.
        using empty_type_adjacency_list = type_adjacency_list<>;
    }

    // Variable template for `type_adjacency_list`.
    template <typename... Ts>
    constexpr impl::type_adjacency_list<Ts...> v{};

    // Variable template for an empty list.
    constexpr impl::empty_type_adjacency_list empty_v{};
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END
