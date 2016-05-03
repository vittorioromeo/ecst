// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/slice.hpp>

ECST_MP_LIST_NAMESPACE
{
    // Unpacks the `rl` list, inserting it into `l` before index `i`.
    template <typename TList, typename TI, typename TInsertList>
    constexpr auto insert_at_by_unpacking(TList l, TI i, TInsertList rl)
    {
        return cat(take_all_before(l, i), rl, take_all_from(l, i));
    }

    // Inserts the `xs...` values before `i` in `l`.
    template <typename TList, typename TI, typename... Ts>
    constexpr auto insert_at(TList l, TI i, Ts... xs)
    {
        return insert_at_by_unpacking(l, i, make(xs...));
    }

    // Pushes the `xs...` values at the beginning of `l`.
    template <typename TList, typename... Ts>
    constexpr auto push_front(TList l, Ts... xs)
    {
        return insert_at(l, sz_v<0>, xs...);
    }

    // Pushes the `xs...` values at the end of `l`.
    template <typename TList, typename... Ts>
    constexpr auto push_back(TList l, Ts... xs)
    {
        return insert_at(l, size(l), xs...);
    }

    // Pops `n` values from the beginning of `l`.
    template <typename TList, typename TN>
    constexpr auto pop_front(TList l, TN n)
    {
        return take_all_from(l, n);
    }

    // Pops `n` values from the beginning of `l`.
    template <typename TList, typename TN>
    constexpr auto pop_back(TList l, TN n)
    {
        return take_all_until(l, sz_v<list::size(l) - decltype(n){} - 1>);
    }

    // Pops one value from the beginning of `l`.
    template <typename TList>
    constexpr auto pop_front(TList l)
    {
        return pop_front(l, sz_v<1>);
    }

    // Pops one value from the beginning of `l`.
    template <typename TList>
    constexpr auto pop_back(TList l)
    {
        return pop_back(l, sz_v<1>);
    }
}
ECST_MP_LIST_NAMESPACE_END
