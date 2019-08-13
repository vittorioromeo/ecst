// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/edge.hpp>
#include <ecst/mp/adjacency_list/mutate_impl.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    /// @brief Emplaces a `(n0, n1, d)` directed edge.
    /// @details Does not check for duplicates.
    template <typename TAList, typename TN0, typename TN1, typename TData>
    constexpr auto emplace_directed_edge(TAList al, TN0 n0, TN1 n1, TData d)
    {
        return decltype(impl::emplace_directed_edge_impl(al, n0, n1, d)){};
    }

    /// @brief Emplaces a `(n0, n1, null_v)` directed edge.
    /// @details Does not check for duplicates.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto emplace_directed_edge(TAList al, TN0 n0, TN1 n1)
    {
        return emplace_directed_edge(al, n0, n1, null_v);
    }

    /// @brief Emplaces the `e` directed edge.
    /// @details Does not check for duplicates.
    template <typename TAList, typename TEdge>
    constexpr auto insert_directed_edge(TAList al, TEdge e)
    {
        ECST_S_ASSERT_DT(edge::valid(e));
        return emplace_directed_edge(al, // .
            edge::start(e), edge::goal(e), edge::data(e));
    }

    namespace impl
    {
        template <typename TAList, typename TN0, typename TN1, typename TData>
        constexpr auto emplace_undirected_edge_impl(
            TAList al, TN0 n0, TN1 n1, TData d)
        {
            auto al0 = emplace_directed_edge(al, n0, n1, d);
            auto al1 = emplace_directed_edge(al0, n1, n0, d);
            return al1;
        }
    }

    /// @brief Emplaces `(n0, n1, d)` and `(n1, n0, d)` directed edges.
    /// @details Does not check for duplicates.
    template <typename TAList, typename TN0, typename TN1, typename TData>
    constexpr auto emplace_undirected_edge(TAList al, TN0 n0, TN1 n1, TData d)
    {
        return decltype(impl::emplace_undirected_edge_impl(al, n0, n1, d)){};
    }

    /// @brief Emplaces `(n0, n1, null_v)`, `(n1, n0, null_v)` directed edges.
    /// @details Does not check for duplicates.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto emplace_undirected_edge(TAList al, TN0 n0, TN1 n1)
    {
        return emplace_undirected_edge(al, n0, n1, null_v);
    }

    /// @brief Emplaces the `e` undirected edge.
    /// @details Does not check for duplicates.
    template <typename TAList, typename TEdge>
    constexpr auto insert_undirected_edge(TAList al, TEdge e)
    {
        ECST_S_ASSERT_DT(edge::valid(e));
        return emplace_undirected_edge(al, // .
            edge::start(e), edge::goal(e), edge::data(e));
    }

    namespace impl
    {
        template <typename TAList, typename TN0List, typename TN1>
        constexpr auto emplace_multiple_directed_edges_starting_from_list_impl(
            TAList al, TN0List n0_list, TN1 n1)
        {
            return list::fold_l(al,
                [=](auto acc_al, auto x_n0)
                {
                    return emplace_directed_edge(acc_al, x_n0, n1);
                },
                n0_list);
        }
    }

    /// @brief Emplaces multiple `n0 -> n1` directed edges, where `n1` is fixed
    /// and all `n0`s are taken from `n0_list`.
    template <typename TAList, typename TN0List, typename TN1>
    constexpr auto emplace_multiple_directed_edges_starting_from_list(
        TAList al, TN0List n0_list, TN1 n1)
    {
        return decltype(
            impl::emplace_multiple_directed_edges_starting_from_list_impl(
                al, n0_list, n1)){};
    }
}

