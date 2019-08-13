// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/mp/pair.hpp>

ECST_MP_EDGE_NAMESPACE
{
    namespace impl
    {
        template <typename TEdge>
        constexpr auto valid_impl(TEdge)
        {
            return bool_v<(pair::valid(list::at(TEdge{}, sz_v<0>)))>;
        }
    }

    /// @brief Returns true if `e` is a valid edge.
    template <typename TEdge>
    constexpr auto valid(TEdge e)
    {
        return decltype(impl::valid_impl(e)){};
    }

    /// @brief Returns the node pair of `e`.
    template <typename TEdge>
    constexpr auto node_pair(TEdge e)
    {
        ECST_S_ASSERT_DT(valid(e));
        return list::at(e, sz_v<0>);
    }

    /// @brief Returns the data of `e`.
    template <typename TEdge>
    constexpr auto data(TEdge e)
    {
        ECST_S_ASSERT_DT(valid(e));
        return list::at(e, sz_v<1>);
    }

    /// @brief Returns the start node of `e`.
    template <typename TEdge>
    constexpr auto start(TEdge e)
    {
        return pair::fst(node_pair(e));
    }

    /// @brief Returns the goal node of `e`.
    template <typename TEdge>
    constexpr auto goal(TEdge e)
    {
        return pair::snd(node_pair(e));
    }

    /// @brief Returns true if the start node of `e` is `n`.
    template <typename TEdge, typename TNode>
    constexpr auto start_is(TEdge e, TNode n)
    {
        return pair::fst_is(node_pair(e), n);
    }

    /// @brief Returns true if the goal node of `e` is `n`.
    template <typename TEdge, typename TNode>
    constexpr auto goal_is(TEdge e, TNode n)
    {
        return pair::snd_is(node_pair(e), n);
    }
}

