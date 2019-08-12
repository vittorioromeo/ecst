// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/ndp.hpp>
#include <ecst/mp/edge.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TAList, typename TNode>
        constexpr auto edges_starting_from_impl(TAList al, TNode)
        {
            auto res = map::find_first_by_key(impl::unwrap(al), TNode{});

            if constexpr(is_null(res))
            {
                return list::empty_v;
            }
            else
            {
                return impl::to_edge_list(TNode{}, res);
            }
        }
    } // namespace impl

    /// @brief Returns a list of edges starting from `n`.
    template <typename TAList, typename TNode>
    constexpr auto edges_starting_from(TAList l, TNode n)
    {
        return decltype(impl::edges_starting_from_impl(l, n)){};
    }

    namespace impl
    {
        template <typename TAList, typename TNode>
        auto neighbors_of_impl(TAList l, TNode n)
        {
            auto edges = edges_starting_from(l, n);
            return list::transform(
                [](auto xe) { return edge::goal(xe); }, edges);
        }
    } // namespace impl

    /// @brief Returns the list of neighbor nodes starting from `n`.
    template <typename TAList, typename TNode>
    constexpr auto neighbors_of(TAList l, TNode n)
    {
        return decltype(impl::neighbors_of_impl(l, n)){};
    }

    /// @brief Returns the count of edges starting from `n`.
    template <typename TAList, typename TNode>
    constexpr auto count_edges_starting_from(TAList l, TNode n)
    {
        return list::size(edges_starting_from(l, n));
    }

    /// @brief Returns a list of all edges connecting `n0` to `n1`.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto edges_connecting(TAList al, TN0 n0, TN1 n1)
    {
        auto esf = edges_starting_from(al, n0);
        return list::filter(
            esf, [=](auto xe) { return edge::goal_is(xe, n1); });
    }

    /// @brief Returns the count of all edges connecting `n0` to `n1`.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto count_edges_connecting(TAList al, TN0 n0, TN1 n1)
    {
        return decltype(list::size(edges_connecting(al, n0, n1))){};
    }

    /// @brief Returns true if any edge connects `n0` to `n1`.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto any_edge_connects(TAList al, TN0 n0, TN1 n1)
    {
        return bool_v<(decltype(count_edges_connecting(al, n0, n1)){} > 0)>;
    }

    /// @brief Returns the number of edges from `n0` to `n1`.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto count_directed_edges(TAList al, TN0 n0, TN1 n1)
    {
        return count_edges_connecting(al, n0, n1);
    }

    /// @brief Returns true if an edge from `n0` to `n1` exists.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto has_directed_edge(TAList, TN0, TN1)
    {
        return bool_v<(                                      // .
            count_directed_edges(TAList{}, TN0{}, TN1{}) > 0 // .
            )>;
    }

    /// @brief Returns true if both an edge from `n0` to `n1` and an edge from
    /// `n1` to `n0` exist.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto has_undirected_edge(TAList, TN0, TN1)
    {
        return bool_v<(                                  // .
            has_directed_edge(TAList{}, TN0{}, TN1{}) && // .
            has_directed_edge(TAList{}, TN1{}, TN0{})    // .
            )>;
    }
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END
