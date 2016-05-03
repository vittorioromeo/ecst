// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/replace_at.hpp>

ECST_MP_LIST_NAMESPACE
{
    // Removes the slice `[lb, ub)` of `l`.
    template <typename TList, typename TLB, typename TUB>
    constexpr auto remove_slice(TList l, TLB lb, TUB ub)
    {
        return replace_slice(l, lb, ub);
    }

    // Removes the item at `i`.
    template <typename TList, typename TI>
    constexpr auto remove_at(TList l, TI i)
    {
        return replace_at(l, i);
    }
}
ECST_MP_LIST_NAMESPACE_END

// TODO: multiple replace/remove target indices overloads
