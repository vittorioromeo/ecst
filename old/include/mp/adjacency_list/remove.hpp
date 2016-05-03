// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/mutate.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TAList, typename TN0, typename TF>
        constexpr auto transform_list_if_existing_at(TAList al, TN0 n0, TF f)
        {
            auto n0i = start_node_idx_or_null(al, n0);
            return static_if(is_null(n0i))
                .then([=](auto xal, auto)
                    {
                        // There is no such edge - do nothing.
                        return xal;
                    })
                .else_([=](auto xal, auto xn0i)
                    {
                        return f(xal, xn0i);
                    })(al, n0i);
        }

        template <typename TAList, typename TN0, typename TN1, typename TData>
        constexpr auto remove_directed_edge_impl(
            TAList al, TN0 n0, TN1 n1, TData d)
        {
            return transform_list_if_existing_at(al, n0,
                [=](auto yal, auto yn0i)
                {
                    return impl::remove_existing_directed_edge(
                        yal, yn0i, n1, d);
                });
        }
    }

    /// @brief Removes the exact `(n0, n1, d)` directed edge, if existent.
    template <typename TAList, typename TN0, typename TN1, typename TData>
    constexpr auto remove_directed_edge(TAList al, TN0 n0, TN1 n1, TData d)
    {
        return decltype(impl::remove_directed_edge_impl(al, n0, n1, d)){};
    }

    /// @brief Removes the exact `(n0, n1, null_v)` directed edge, if existent.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto remove_directed_edge(TAList al, TN0 n0, TN1 n1)
    {
        return remove_directed_edge(al, n0, n1, null_v);
    }

    namespace impl
    {
        template <typename TAList, typename TN0, typename TN1, typename TData>
        constexpr auto remove_undirected_edge_impl(
            TAList al, TN0 n0, TN1 n1, TData d)
        {
            auto al0 = remove_directed_edge(al, n0, n1, d);
            auto al1 = remove_directed_edge(al0, n1, n0, d);
            return al1;
        }
    }

    /// @brief Removes `(n0, n1, d)`, `(n1, n0, d)` edges, if existent.
    /// @details Does not check for undirected edge validity or duplicates.
    template <typename TAList, typename TN0, typename TN1, typename TData>
    constexpr auto remove_undirected_edge(TAList al, TN0 n0, TN1 n1, TData d)
    {
        return decltype(impl::remove_undirected_edge_impl(al, n0, n1, d)){};
    }

    /// @brief Removes `(n0, n1, null_v)` and `(n1, n0, null_v)` if existent.
    /// @details Does not check for undirected edge validity or duplicates.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto remove_undirected_edge(TAList al, TN0 n0, TN1 n1)
    {
        return remove_undirected_edge(al, n0, n1, null_v);
    }

    namespace impl
    {
        template <typename TAList, typename TN0, typename TN1>
        constexpr auto remove_all_directed_edges_impl(TAList al, TN0 n0, TN1 n1)
        {
            return transform_list_if_existing_at(al, n0,
                [=](auto yal, auto yn0i)
                {
                    return transform_existing_list(yal, yn0i, [=](auto xold_l)
                        {
                            return list::remove_matching(xold_l, [=](auto xndp)
                                {
                                    return same_type_decay(n1, ndp_goal(xndp));
                                });
                        });
                });
        }
    }

    /// @brief Removes all `(n0, n1, _)` directed edges.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto remove_all_directed_edges(TAList al, TN0 n0, TN1 n1)
    {
        return decltype(impl::remove_all_directed_edges_impl(al, n0, n1)){};
    }

    namespace impl
    {
        template <typename TAList, typename TN0, typename TN1>
        constexpr auto remove_all_undirected_edges_impl(
            TAList al, TN0 n0, TN1 n1)
        {
            auto al0 = remove_all_directed_edges(al, n0, n1);
            auto al1 = remove_all_directed_edges(al0, n1, n0);
            return al1;
        }
    }

    /// @brief Removes all `(n0, n1, _)` and `(n1, n0, _)` edges.
    template <typename TAList, typename TN0, typename TN1>
    constexpr auto remove_all_undirected_edges(TAList al, TN0 n0, TN1 n1)
    {
        return decltype(impl::remove_all_undirected_edges_impl(al, n0, n1)){};
    }

    namespace impl
    {
        template <typename TAList, typename TN0>
        constexpr auto remove_all_edges_starting_from_impl(TAList al, TN0 n0)
        {
            return transform_list_if_existing_at(al, n0,
                [=](auto yal, auto yn0i)
                {
                    return replace_edge_list_at(yal, yn0i, list::empty_v);
                });
        }
    }

    template <typename TAList, typename TN0>
    constexpr auto remove_all_edges_starting_from(TAList al, TN0 n0)
    {
        return decltype(impl::remove_all_edges_starting_from_impl(al, n0)){};
    }
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END

// TODO:
// * remove_all_edges_arriving_to(n1)
// * remove_all_edges_with_data(d)
