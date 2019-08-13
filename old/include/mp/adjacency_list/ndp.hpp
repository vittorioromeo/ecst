// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/edge.hpp>
#include <ecst/mp/adjacency_list/basic.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TNodeDataPair>
        constexpr auto ndp_goal(TNodeDataPair ndp)
        {
            return pair::fst(ndp);
        }

        template <typename TNodeDataPair>
        constexpr auto ndp_data(TNodeDataPair ndp)
        {
            return pair::snd(ndp);
        }

        template <typename TNode, typename TNodeDataPair>
        constexpr auto to_edge(TNode n, TNodeDataPair ndp)
        {
            return edge::make_data(n, ndp_goal(ndp), ndp_data(ndp));
        }

        template <typename TNode, typename TNDPList>
        constexpr auto to_edge_list(TNode n, TNDPList ndp_list)
        {
            return list::transform(
                [=](auto x)
                {
                    return to_edge(n, x);
                },
                ndp_list);
        }
    }
}

