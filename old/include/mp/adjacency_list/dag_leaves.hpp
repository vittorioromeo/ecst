// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/all_nodes.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TAList>
        auto dag_leaves_impl(TAList al)
        {
            return list::remove_matching(all_nodes(al), [=](auto zn)
                {
                    return has_start_node(al, zn);
                });
        }
    }

    /// @brief Returns a list of nodes that are only goals.
    /// @details Assumes the graph is acyclical.
    template <typename TAList>
    constexpr auto dag_leaves(TAList al)
    {
        return decltype(impl::dag_leaves_impl(al)){};
    }
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END
