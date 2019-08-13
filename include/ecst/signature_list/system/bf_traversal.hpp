// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/system/dependents.hpp>

namespace ecst::signature_list::system
{
    namespace bf_traversal
    {
        using namespace mp;

        template <typename TStartNodeList>
        constexpr auto make(TStartNodeList&& snl) noexcept
        {
            return bh::make_pair(FWD(snl), list::empty_v);
        }

        template <typename TBFTContext>
        constexpr auto queue(TBFTContext&& c) noexcept
        {
            return bh::first(FWD(c));
        }

        template <typename TBFTContext>
        constexpr auto visited(TBFTContext&& c) noexcept
        {
            return bh::second(FWD(c));
        }

        template <typename TBFTContext, typename TNode>
        constexpr auto is_visited(TBFTContext&& c, TNode&& n) noexcept
        {
            return bh::contains(visited(FWD(c)), FWD(n));
        }

        template <typename TBFTContext, typename TNode>
        constexpr auto is_in_queue(TBFTContext&& c, TNode&& n) noexcept
        {
            return bh::contains(queue(FWD(c)), FWD(n));
        }

        template <typename TBFTContext>
        constexpr auto is_queue_empty(TBFTContext&& c) noexcept
        {
            return bh::is_empty(queue(FWD(c)));
        }

        template <typename TBFTContext>
        constexpr auto top_node(TBFTContext&& c) noexcept
        {
            return bh::front(queue(FWD(c)));
        }

        template <typename TBFTContext, typename TSSL>
        auto step_forward(TBFTContext&& c, TSSL ssl) noexcept
        {
            auto node = top_node(c);
            auto popped_queue = bh::remove_at(queue(c), mp::sz_v<0>);
            auto neighbors =
                dependent_ids_list(ssl, signature_by_id(ssl, node));

            auto unvisited_neighbors = bh::remove_if(
                neighbors, [=](auto x_nbr) { return is_visited(c, x_nbr); });

            auto new_visited = bh::concat(visited(c), unvisited_neighbors);
            auto new_queue = bh::concat(popped_queue, unvisited_neighbors);

            return bh::make_pair(new_queue, new_visited);
        }

        template <typename TStartNodeList, typename TSSL>
        auto execute(TStartNodeList&& snl, TSSL ssl) noexcept
        {
            using namespace mp;
            namespace btfc = bf_traversal;

            auto step = [=](auto self, auto&& ctx) {
                if constexpr(btfc::is_queue_empty(
                                 std::decay_t<decltype(ctx)>{}))
                {
                    return list::empty_v;
                }
                else
                {
                    return bh::append(self(btfc::step_forward(FWD(ctx), ssl)),
                        btfc::top_node(FWD(ctx)));
                }
            };

            return bh::fix(step)(btfc::make(FWD(snl)));
        }
    } // namespace bf_traversal
} // namespace ecst::signature_list::system
