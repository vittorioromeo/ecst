// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/adjacency_list/mutate.hpp>
#include <ecst/mp/adjacency_list/start_nodes.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace bf_traversal_context
    {
        namespace impl
        {
            template <typename TAList>
            auto make_impl(TAList al)
            {
                auto snl = start_node_list(al);
                return pair::make(snl, snl);
            }
        } // namespace impl

        template <typename TAList>
        constexpr auto make(TAList al)
        {
            return decltype(impl::make_impl(al)){};
        }

        template <typename TBFTContext>
        constexpr auto queue(TBFTContext c)
        {
            return pair::fst(c);
        }

        template <typename TBFTContext>
        constexpr auto visited(TBFTContext c)
        {
            return pair::snd(c);
        }

        template <typename TBFTContext, typename TNode>
        constexpr auto is_visited(TBFTContext c, TNode n)
        {
            return list::contains(visited(c), n);
        }

        template <typename TBFTContext, typename TNode>
        constexpr auto is_in_queue(TBFTContext c, TNode n)
        {
            return list::contains(queue(c), n);
        }

        template <typename TBFTContext>
        constexpr auto is_queue_empty(TBFTContext c)
        {
            return list::empty(queue(c));
        }

        template <typename TBFTContext>
        constexpr auto top_node(TBFTContext c)
        {
            return list::head(queue(c));
        }

        namespace impl
        {
            template <typename TBFTContext, typename TAList>
            auto step_forward_impl(TBFTContext c, TAList al)
            {
                auto node = top_node(c);
                auto popped_queue = list::pop_front(queue(c));
                auto neighbors = neighbors_of(al, node);

                auto unvisited_neighbors = list::remove_matching(neighbors,
                    [=](auto x_nbr) { return is_visited(c, x_nbr); });

                auto new_visited = list::cat(visited(c), unvisited_neighbors);
                auto new_queue = list::cat(popped_queue, unvisited_neighbors);

                return pair::make(new_queue, new_visited);
            }
        } // namespace impl

        template <typename TBFTContext, typename TAList>
        constexpr auto step_forward(TBFTContext c, TAList al)
        {
            return decltype(impl::step_forward_impl(c, al)){};
        }
    } // namespace bf_traversal_context

    namespace impl
    {
        template <typename TAList, typename TF>
        auto runtime_bf_traversal_impl(TAList al, TF f)
        {
            namespace btfc = bf_traversal_context;

            auto step = [=](auto self, auto ctx) {
                if constexpr(!btfc::is_queue_empty(ctx))
                {
                    auto node = btfc::top_node(ctx);
                    f(node);

                    return self(btfc::step_forward(ctx, al));
                };
            };

            y_combinator(step)(btfc::make(al));
        }
    } // namespace impl

    template <typename TAList, typename TF>
    void runtime_bf_traversal(TAList al, TF f)
    {
        impl::runtime_bf_traversal_impl(al, f);
    }
}
ECST_MP_ADJACENCY_LIST_NAMESPACE_END
