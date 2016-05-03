// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/minmax.hpp>
#include <ecst/mp/list/replace_at.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        /// @brief Returns a list of indices `{lb, ... lb + TIs}`.
        template <typename TLB, sz_t... TIs>
        constexpr auto make_index_list_impl_expand(
            TLB lb, std::index_sequence<TIs...>)
        {
            (void)lb;
            return make(sz_v<lb + TIs>...);
        }

        /// @brief Returns a list of indices `[lb, ub)`.
        template <typename TLB, typename TUB>
        constexpr auto make_index_list_impl(TLB lb, TUB ub)
        {
            return make_index_list_impl_expand(
                lb, std::make_index_sequence<ub - lb>());
        }
    }

    /// @brief Returns a list of indices `[0, ub)`.
    template <typename TUB>
    constexpr auto make_index_list(TUB ub)
    {
        return impl::make_index_list_impl(sz_v<0>, ub);
    }

    /// @brief Returns a list of indices `[lb, ub)`.
    template <typename TLB, typename TUB>
    constexpr auto make_index_list(TLB lb, TUB ub)
    {
        return impl::make_index_list_impl(lb, ub);
    }

    /// @brief Returns a list of indices `[0, size(l))`.
    template <typename TList>
    constexpr auto make_index_list_for(TList l)
    {
        return make_index_list(size(l));
    }

    /// @brief Returns a list of indices that starts from `0` and goes to the
    /// minimum size between `ls...`.
    template <typename... TLists>
    constexpr auto make_minimal_index_list_for(TLists... ls)
    {
        return make_index_list(::ecst::mp::min<sz_t>(size(ls)...));
    }

    /// @brief Returns a list of indices that starts from `0` and goes to the
    /// minimum size of the elements of `l`.
    template <typename TListList>
    constexpr auto make_minimal_index_list_for_by_unpacking(TListList l)
    {
        return apply(
            [](auto... ls)
            {
                return make_minimal_index_list_for(ls...);
            },
            l);
    }
}
ECST_MP_LIST_NAMESPACE_END
