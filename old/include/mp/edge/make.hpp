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
    /// @brief Returns a graph edge with data, from a pair of nodes.
    template <typename TNodePair, typename TData>
    constexpr auto make_data_from_pair(TNodePair np, TData d)
    {
        return list::make(np, d);
    }

    /// @brief Returns a graph edge with data, from two nodes.
    template <typename TN0, typename TN1, typename TData>
    constexpr auto make_data(TN0 n0, TN1 n1, TData d)
    {
        return make_data_from_pair(pair::make(n0, n1), d);
    }

    /// @brief Returns a null-data graph edge, from a pair of nodes.
    template <typename TNodePair>
    constexpr auto make_from_pair(TNodePair np)
    {
        return make_data_from_pair(np, null_v);
    }

    /// @brief Returns an null-data graph edge, from two nodes.
    template <typename TN0, typename TN1>
    constexpr auto make(TN0 n0, TN1 n1)
    {
        return make_from_pair(pair::make(n0, n1));
    }
}
ECST_MP_EDGE_NAMESPACE_END
