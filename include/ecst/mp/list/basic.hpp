// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list/types.hpp>
#include <ecst/mp/list/is_list.hpp>

ECST_MP_LIST_NAMESPACE
{
    // Returns a list created from the passed argument types.
    template <typename... Ts>
    constexpr auto make(Ts...)
    {
        return v<Ts...>;
    }

    /// @brief Returns true if `l` is a valid list.
    template <typename TList>
    constexpr auto valid(TList l)
    {
        return is_list(l);
    }

    // Returns the size of a list.
    template <typename TList>
    constexpr auto size(TList l)
    {
        ECST_S_ASSERT_DT(valid(l));
        return bh::length(l);
    }

    namespace impl
    {
        template <typename TList>
        constexpr auto empty_impl(TList l)
        {
            return size(l) == 0;
        }
    }

    // Returns whether a list is empty or not.
    template <typename TList>
    constexpr auto empty(TList l)
    {
        return decltype(impl::empty_impl(l)){};
    }

    template <typename... Ts>
    constexpr auto all_variadic(Ts && ... xs)
    {
        return bh::all(bh::make_basic_tuple(FWD(xs)...));
    }

    // Concatenates multiple lists together.
    template <typename... TLists>
    constexpr auto cat(TLists... ls)
    {
        ECST_S_ASSERT_DT(all_variadic(valid(ls)...));
        return bh::concat(ls...);
    }

    // Appends some items at the end of a list.
    template <typename TList, typename... Ts>
    constexpr auto append(TList l, Ts... xs)
    {
        return cat(l, make(xs...));
    }

    // Prepends some items at the end of a list.
    template <typename TList, typename... Ts>
    constexpr auto prepend(TList l, Ts... xs)
    {
        return cat(make(xs...), l);
    }

    namespace impl
    {
        template <typename TList, typename TI>
        constexpr auto valid_index_impl(TList, TI)
        {
            return true;
        }
    }

    // Returns true if `i` is a valid index for `l`.
    template <typename TList, typename TI>
    constexpr auto valid_index(TList l, TI i)
    {
        return decltype(impl::valid_index_impl(l, i)){};
    }

    // Returns the `i`-th item of a list.
    template <typename TList, typename TI>
    constexpr auto at(TList l, TI i)
    {
        // ECST_S_ASSERT_DT(valid_index(l, i));
        return bh::at(l, i);
    }

    // Returns the first item of `l`.
    template <typename TList>
    constexpr auto head(TList l)
    {
        return at(l, sz_v<0>);
    }

    // Returns the last valid index of `l`.
    template <typename TList>
    constexpr auto last_index(TList l)
    {
        ECST_S_ASSERT_DT(!empty(l));
        return size(l) - 1;
    }

    // Returns the last item of `l`.
    template <typename TList>
    constexpr auto tail(TList l)
    {
        return at(l, last_index(l));
    }
}
ECST_MP_LIST_NAMESPACE_END
