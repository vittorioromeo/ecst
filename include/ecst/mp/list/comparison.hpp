// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/minmax.hpp>
#include <ecst/mp/list/fold.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TList, typename... TLists>
        constexpr auto equal_size_impl(TList, TLists...)
        {
            return conjugate(bool_v<(size(TList{}) == size(TLists{}))>...);
        }
    }

    /// @brief Returns true if all `ls...` are of the same size.
    template <typename... TLists>
    constexpr auto equal_size(TLists... ls)
    {
        return decltype(impl::equal_size_impl(ls...)){};
    }

    /// @brief Returns true if all `ls...` are identical.
    template <typename TList, typename... TLists>
    constexpr auto equal(TList, TLists...)
    {
        return conjugate(bool_v<same_type(TList{}, TLists{})>...);
    }

    /// @brief Returns true if all lists in `ll` are of the same size.
    template <typename TListList>
    constexpr auto equal_by_unpacking(TListList ll)
    {
        return apply(
            [](auto... ls)
            {
                return equal(ls...);
            },
            ll);
    }
}
ECST_MP_LIST_NAMESPACE_END
