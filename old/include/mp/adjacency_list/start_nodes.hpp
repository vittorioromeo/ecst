// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/map.hpp>
#include <ecst/mp/adjacency_list/basic.hpp>
#include <ecst/mp/adjacency_list/wrap.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    /// @brief Returns the number of start nodes in `al`.
    template <typename TAList>
    constexpr auto start_node_count(TAList al)
    {
        ECST_S_ASSERT_DT(valid(al));
        return map::size(impl::unwrap(al));
    }

    /// @brief Returns the list of start nodes in `al`.
    template <typename TAList>
    constexpr auto start_node_list(TAList al)
    {
        ECST_S_ASSERT_DT(valid(al));
        return map::key_list(impl::unwrap(al));
    }

    /// @brief Returns true if `n` is a start node in `al`.
    template <typename TAList, typename TNode>
    constexpr auto has_start_node(TAList al, TNode n)
    {
        return list::contains(start_node_list(al), n);
    }
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END
