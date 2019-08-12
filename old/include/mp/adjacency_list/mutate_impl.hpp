// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/edges.hpp>
#include <ecst/mp/edge.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TAList, typename TN0, typename TNDPair>
        constexpr auto add_goal_to_new_start(TAList al, TN0 n0, TNDPair ndp)
        {
            // Assumes there is no such `n0` start node.
            static_assert(!decltype(has_start_node(al, n0)){});

            // Create key-value pair.
            auto ual = impl::unwrap(al);
            auto new_ual = map::emplace(ual, n0, list::make(ndp));

            return impl::wrap(new_ual);

            // TODO: return?
            // auto new_e = to_edge(n0, ndp);
        }

        /// @brief Returns the edge list at index `n0i`.
        /// @details Assumes the edge list exists.
        template <typename TAList, typename TN0Idx>
        constexpr auto edge_list_at(TAList al, TN0Idx n0i)
        {
            auto ual = impl::unwrap(al);

            // Get edge list from key index `n0i`.
            auto kv = list::at(ual, n0i);
            auto l = pair::snd(kv);

            return l;
        }

        /// @brief Replaces the edge list at index `n0i` with `new_el`.
        /// @details Returns a wrapped adjacency list.
        template <typename TAList, typename TN0Idx, typename TEL>
        constexpr auto replace_edge_list_at(TAList al, TN0Idx n0i, TEL new_el)
        {
            auto ual = impl::unwrap(al);
            auto kv = list::at(ual, n0i);

            // Replace old edge list with new one.
            auto new_kv = pair::replace_snd(kv, new_el);
            auto res = list::replace_at(ual, n0i, new_kv);

            return impl::wrap(res);
        }

        /// @brief Applies `f` to the existing list of edges starting from `n0`.
        /// @details `n0i` is the index of the kv-pair with key `n0`. Returns a
        /// wrapped adjacency list.
        template <typename TAList, typename TN0Idx, typename TF>
        constexpr auto transform_existing_list(TAList al, TN0Idx n0i, TF f)
        {
            // Get edge list from key index `n0i`.
            auto old_l = edge_list_at(al, n0i);

            // Apply `f` to edge list.
            return replace_edge_list_at(al, n0i, f(old_l));
        }

        /// @brief Adds a goal-data pair to an existing edge list at `n0i`.
        /// @details Returns a wrapped adjacency list.
        template <typename TAList, typename TN0Idx, typename TNDPair>
        constexpr auto add_goal_to_existing_start(
            TAList al, TN0Idx n0i, TNDPair ndp)
        {
            return transform_existing_list(al, n0i,
                [=](auto xold_l) { return list::append(xold_l, ndp); });
        }

        /// @brief Removes an existing goal-data pair from the edge list at
        /// `n0i`.
        /// @details Returns a wrapped adjacency list.
        template <typename TAList, typename TN0Idx, typename TN1,
            typename TData>
        constexpr auto remove_existing_directed_edge(
            TAList al, TN0Idx n0i, TN1 n1, TData d)
        {
            auto target_ndp = pair::make(n1, d);
            return transform_existing_list(al, n0i, [=](auto xold_l) {
                return list::remove_matching(xold_l, [=](auto xndp) {
                    return same_type_decay(target_ndp, xndp);
                });
            });
        }

        /// @brief Returns the index of `n0`, if it exists, `null_v` otherwise.
        template <typename TAList, typename TN0>
        constexpr auto start_node_idx_or_null(TAList al, TN0 n0)
        {
            return list::find_first_index_of_matching(
                impl::unwrap(al), [=](auto xnl) {
                    // Find bucket with key `n0`.
                    return same_type_decay(pair::fst(xnl), n0);
                });
        }

        /// @brief Emplaces a `(n0, n1, d)` directed edge.
        /// @details The "node/edge-list" kv-pair is created if non-existent.
        template <typename TAList, typename TN0, typename TN1, typename TData>
        constexpr auto emplace_directed_edge_impl(
            TAList al, TN0 n0, TN1 n1, TData d)
        {
            ECST_S_ASSERT_DT(valid(al));

            // Check if start node is already in adjacency list.
            auto n0i = start_node_idx_or_null(al, n0);

            // Goal node and data pair.
            auto gdp = pair::make(n1, d);

            if constexpr(is_null(n0i))
            {
                // If there was no such bucket, create one:
                // `(start_node, [(goal_node, data)]`

                return add_goal_to_new_start(al, n0, gdp);
            }
            else
            {
                return add_goal_to_existing_start(al, n0i, gdp);
            }
        }
    } // namespace impl
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END
