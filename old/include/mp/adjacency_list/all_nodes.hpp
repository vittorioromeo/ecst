// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/start_nodes.hpp>
#include <ecst/mp/adjacency_list/mutate.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TAList>
        auto all_nodes_impl(TAList al)
        {
            return list::fold_l(start_node_list(al),
                [=](auto acc, auto xn)
                {
                    return list::fold_l(acc,
                        [=](auto yacc, auto yn)
                        {
                            return list::push_back_if_non_existant(yacc, yn);
                        },
                        neighbors_of(al, pair::fst(xn)));
                },
                al);
        }
    }

    /// @brief Returns a list of all nodes inside the adjacency list, without
    /// duplicates.
    template <typename TAList>
    constexpr auto all_nodes(TAList al)
    {
        return decltype(impl::all_nodes_impl(al)){};
    }

    /// @brief Returns the count of all nodes inside the adjacency list, without
    /// duplicates.
    template <typename TAList>
    constexpr auto node_count(TAList al)
    {
        return list::size(all_nodes(al));
    }
}

