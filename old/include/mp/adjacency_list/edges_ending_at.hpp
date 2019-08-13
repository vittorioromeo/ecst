// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/edges.hpp>
#include <ecst/mp/adjacency_list/ndp.hpp>
#include <ecst/mp/edge.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TNDPList, typename TNode>
        constexpr auto find_ndp_with_goal_from_ndl_list(TNDPList ndpl, TNode n)
        {
            return list::find_if(ndpl,
                [=](auto ndp) { return same_type_decay(ndp_goal(ndp), n); });
        }

        template <typename TALKVP, typename TNode>
        constexpr auto al_kvp_has_goal(TALKVP al_kvp, TNode n)
        {
            return bool_v<!decltype(is_null(
                find_ndp_with_goal_from_ndl_list(map::value(al_kvp), n))){}>;
        }

        template <typename TAList, typename TNode>
        constexpr auto edges_ending_at_impl(TAList al, TNode n)
        {
            return list::fold_l(
                list::empty_v,
                [=](auto acc, auto x_kvp) {
                    // Find `n` in the list of goals of `x_kvp`.
                    auto ndp =
                        find_ndp_with_goal_from_ndl_list(map::value(x_kvp), n);

                    if constexpr(is_null(ndp))
                    {
                        return acc;
                    }
                    else
                    {
                        return list::append(acc, to_edge(map::key(x_kvp), ndp));
                    };
                },
                impl::unwrap(al));
        }
    } // namespace impl

    /// @brief Returns a list of edges ending at `n`.
    /// @details Does not currently support multiple edges with same start/goal
    /// nodes (TODO).
    template <typename TAList, typename TNode>
    constexpr auto edges_ending_at(TAList al, TNode n)
    {
        return decltype(impl::edges_ending_at_impl(al, n)){};
    }

    /// @brief Returns the count of edges ending at `n`.
    template <typename TAList, typename TNode>
    constexpr auto count_edges_ending_at(TAList al, TNode n)
    {
        return list::size(edges_ending_at(al, n));
    }
}

