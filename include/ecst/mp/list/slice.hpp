// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/pick.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TOffset, sz_t... TIs>
        constexpr auto list_from_index_sequence(
            TOffset, std::index_sequence<TIs...>)
        {
            return make(sz_v<TOffset{} + TIs>...);
        }

        template <sz_t... TIs>
        constexpr auto make_index_sequence_list_impl(
            std::index_sequence<TIs...>)
        {
            return make(sz_v<TIs>...);
        }

        template <sz_t TN>
        constexpr auto make_index_sequence_list()
        {
            return make_index_sequence_list_impl(
                std::make_index_sequence<TN>());
        }

        template <typename TList, typename TLB, typename TUB>
        constexpr auto valid_interval_impl(TList, TLB lb, TUB ub)
        {
            return bool_v<(            // .
                lb >= 0 && lb <= ub && // .
                ub >= 0 && lb <= ub && // .
                ub <= size(TList{})    // .
                )>;
        }
    }

    // Returns whether or not `[lb, ub)` is a valid interval for `l`.
    template <typename TList, typename TLB, typename TUB>
    constexpr auto valid_interval(TList l, TLB lb, TUB ub)
    {
        return decltype(impl::valid_interval_impl(l, lb, ub)){};
    }

    // Returns whether or not `[lb, ub)` is an empty interval for `l`.
    template <typename TList, typename TLB, typename TUB>
    constexpr auto empty_interval(TList l, TLB lb, TUB ub)
    {
        ECST_S_ASSERT_DT(valid_interval(l, lb, ub));
        return bool_v<(lb >= ub)>;
    }

    // Returns a list of `sz_v` items going as such `[lb, ub)`.
    template <typename TLB, typename TUB>
    constexpr auto make_interval_sequence(TLB lb, TUB ub)
    {
        return impl::list_from_index_sequence(
            lb, std::make_index_sequence<ub - lb>());
    }

    // Returns the sub-list `[lb, ub)` of `l`.
    template <typename TList, typename TLB, typename TUB>
    constexpr auto slice(TList l, TLB lb, TUB ub)
    {
        ECST_S_ASSERT_DT(valid_interval(l, lb, ub));
        return pick_at_by_unpacking(l, make_interval_sequence(lb, ub));
    }

    // Returns the first `n` items of `l` in a list.
    template <typename TList, typename TN>
    constexpr auto take_first_n(TList l, TN n)
    {
        return slice(l, sz_v<0>, n);
    }

    // Returns the last `n` items of `l` in a list.
    template <typename TList, typename TN>
    constexpr auto take_last_n(TList l, TN n)
    {
        constexpr auto lb = size(l) - n;
        return slice(l, lb, lb + n);
    }

    // Returns `[n, ...]` items of `l` in a list.
    template <typename TList, typename TI>
    constexpr auto take_all_from(TList l, TI i)
    {
        return slice(l, i, size(l));
    }

    // Returns `(n, ...]` items of `l` in a list.
    template <typename TList, typename TI>
    constexpr auto take_all_after(TList l, TI i)
    {
        return take_all_from(l, sz_v<i + 1>);
    }

    // Returns `[..., n]` items of `l` in a list.
    template <typename TList, typename TI>
    constexpr auto take_all_until(TList l, TI i)
    {
        return take_first_n(l, sz_v<i + 1>);
    }

    // Returns `[..., n)` items of `l` in a list.
    template <typename TList, typename TI>
    constexpr auto take_all_before(TList l, TI i)
    {
        return take_first_n(l, i);
    }
}
ECST_MP_LIST_NAMESPACE_END
