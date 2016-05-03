// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>
#include <ecst/mp/list/inverse_indices.hpp>
#include <ecst/mp/list/remove.hpp>

ECST_MP_LIST_NAMESPACE
{
    /// @brief Returns a list with all the elements of `l` that satisfy `c`.
    template <typename TList, typename TComparer>
    constexpr auto filter(TList l, TComparer c)
    {
        auto indices_to_keep(find_indices_of_matching(l, c));
        return pick_at_by_unpacking(l, indices_to_keep);
    }

    /// @brief Returns a list with all the elements of `l` that do not satisfy
    /// `c`.
    template <typename TList, typename TComparer>
    constexpr auto remove_matching(TList l, TComparer c)
    {
        return filter(l, negate(c));
    }

    /// @brief Removes the first occurrence of `x`.
    /// @details Assumes at least one occurrence of `x` is in `l`.
    template <typename TList, typename T>
    constexpr auto remove_first(TList l, T x)
    {
        return list::remove_at(l, list::find_first_index_of(l, x));
    }
}
ECST_MP_LIST_NAMESPACE_END
