// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/minmax.hpp>
#include <ecst/mp/list/count.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TIndexList, typename... TIs>
        constexpr auto inverse_indices_impl(TIndexList il, TIs... is)
        {
            auto is_list(make(is...));
            return fold_l_with_index(empty_v,
                [=](auto i, auto acc, auto)
                {
                    return static_if(contains(is_list, i))
                        .then([=](auto x_acc)
                            {
                                return x_acc;
                            })
                        .else_([=](auto x_acc)
                            {
                                return append(x_acc, i);
                            })(acc);
                },
                il);
        }
    }

    /// @brief Returns a list of all the indices of `l` except `is...`.
    template <typename TList, typename... TIs>
    constexpr auto inverse_indices(TList l, TIs... is)
    {
        return impl::inverse_indices_impl(make_index_list_for(l), is...);
    }

    /// @brief Returns a list of all the indices of `l` except those in `il`.
    template <typename TList, typename TIndexList>
    constexpr auto inverse_indices_by_unpacking(TList l, TIndexList il)
    {
        return apply(
            [=](auto... is)
            {
                return inverse_indices(l, is...);
            },
            il);
    }

    /// @brief Returns a list containing all elements of `l` except those at
    /// `is...`.
    template <typename TList, typename... TIs>
    constexpr auto pick_all_except_at(TList l, TIs... is)
    {
        auto indices_to_keep(inverse_indices(l, is...));
        return pick_at_by_unpacking(l, indices_to_keep);
    }

    /// @brief Returns a list containing all elements of `l` except those at
    /// the indices in `il`.
    template <typename TList, typename TIndexList>
    constexpr auto pick_all_except_at_by_unpacking(TList l, TIndexList il)
    {
        return apply(
            [=](auto... is)
            {
                return pick_all_except_at(l, is...);
            },
            il);
    }
}
ECST_MP_LIST_NAMESPACE_END
