// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/basic.hpp>

ECST_MP_LIST_NAMESPACE
{
    // Returns a new list containing the values picked at the specified
    // indices from the original list.
    template <typename TList, typename... TIs>
    constexpr auto pick_at(TList l, TIs... is)
    {
        (void)l;
        return make(at(l, is)...);
    }

    namespace impl
    {
        template <typename TList, typename TIndexList>
        constexpr auto pick_at_by_unpacking_apply_impl(TList l, TIndexList il)
        {
            return apply(
                [l](auto... xis)
                {
                    return pick_at(l, xis...);
                },
                il);
        }

        template <typename TList, typename TIndexList>
        constexpr auto pick_at_by_unpacking_impl(TList l, TIndexList il)
        {
            return decltype(pick_at_by_unpacking_apply_impl(l, il)){};
        }
    }

    template <typename TList, typename TIndexList>
    constexpr auto pick_at_by_unpacking(TList l, TIndexList il)
    {
        return impl::pick_at_by_unpacking_impl(l, il);
    }
}
ECST_MP_LIST_NAMESPACE_END
