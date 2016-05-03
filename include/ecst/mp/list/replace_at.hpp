// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/mutate.hpp>

ECST_MP_LIST_NAMESPACE
{
    // TODO: multiple replace/remove target indices overloads

    namespace impl
    {
        template <typename TList, typename TLB, typename TUB,
            typename TReplaceList>
        constexpr auto replace_slice_by_unpacking_impl(
            TList l, TLB lb, TUB ub, TReplaceList rl)
        {
            return static_if(bool_v<empty_interval(TList{}, lb, ub)>)
                .then([=](auto x)
                    {
                        return x;
                    })
                .else_([=](auto x)
                    {
                        return cat(
                            take_all_before(x, lb), rl, take_all_from(x, ub));
                    })(decltype(l){});
        }
    }

    // Replaces the slice `[lb, ub)` of `l`  with the contents of `rl`.
    template <typename TList, typename TLB, typename TUB, typename TReplaceList>
    constexpr auto replace_slice_by_unpacking(
        TList l, TLB lb, TUB ub, TReplaceList rl)
    {
        return decltype(impl::replace_slice_by_unpacking_impl(l, lb, ub, rl)){};
    }

    // Replaces the slice `[lb, ub)` of `l`  with `xs...`.
    template <typename TList, typename TLB, typename TUB, typename... Ts>
    constexpr auto replace_slice(TList l, TLB lb, TUB ub, Ts... xs)
    {
        return replace_slice_by_unpacking(l, lb, ub, make(xs...));
    }

    // Replaces the item at index `i` with the contents of `rl`.
    template <typename TList, typename TI, typename TReplaceList>
    constexpr auto replace_at_by_unpacking(TList l, TI i, TReplaceList rl)
    {
        return replace_slice_by_unpacking(l, i, sz_v<i + 1>, rl);
    }

    // Replaces the item at index `i` with all `xs...`.
    template <typename TList, typename TI, typename... Ts>
    constexpr auto replace_at(TList l, TI i, Ts... xs)
    {
        return replace_at_by_unpacking(l, i, make(xs...));
    }
}
ECST_MP_LIST_NAMESPACE_END
