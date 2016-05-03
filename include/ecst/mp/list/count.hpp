// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/minmax.hpp>
#include <ecst/mp/list/contains.hpp>
#include <ecst/mp/list/find.hpp>

ECST_MP_LIST_NAMESPACE
{
    /// @brief Returns the count of elements in `l` that satisfy `c`.
    template <typename TList, typename TComparer>
    constexpr auto count_matching(TList l, TComparer c)
    {
        return size(find_indices_of_matching(l, c));
    }

    /// @brief Returns true if any element in `l` satisfies `c`.
    template <typename TList, typename TComparer>
    constexpr auto contains_matching(TList l, TComparer c)
    {
        return bool_v<(count_matching(l, c) > 0)>;
    }

    /// @brief Returns the count of elements in `l` equal to `T`.
    template <typename TList, typename T>
    constexpr auto count(TList l, T)
    {
        return count_matching(l, equal_to<T>);
    }

    template <typename TList, typename TComparer>
    constexpr auto all_match(TList l, TComparer c)
    {
        (void)l;
        return bool_v<(count_matching(TList{}, c) == size(TList{}))>;
    }

    namespace impl
    {
        template <typename TList, typename TListToCheck>
        constexpr auto contains_all_by_unpacking_impl(TList l, TListToCheck ltc)
        {
            return list::fold_l(bool_v<true>,
                [=](auto acc, auto x_itc)
                {
                    auto contains_res = list::contains(l, x_itc);
                    return bool_v<(acc && decltype(contains_res){})>;
                },
                ltc);
        }
    }

    /// @brief Returns true if all elements in `ltc` are contained in `l`.
    template <typename TList, typename TListToCheck>
    constexpr auto contains_all_by_unpacking(TList l, TListToCheck ltc)
    {
        return decltype(impl::contains_all_by_unpacking_impl(l, ltc)){};
    }

    /// @brief Returns true if all `xs...` are contained in `l`.
    template <typename TList, typename... Ts>
    constexpr auto contains_all(TList l, Ts... xs)
    {
        return contains_all_by_unpacking(l, list::make(xs...));
    }

    namespace impl
    {
        template <typename TList, typename T>
        constexpr auto push_back_if_non_existant_impl(TList l, T x)
        {
            return static_if(contains(l, x))
                .then([](auto xl)
                    {
                        return xl;
                    })
                .else_([=](auto xl)
                    {
                        return push_back(xl, x);
                    })(l);
        }
    }

    /// @brief Pushes `x` back into `l` only if `l` does not contain `x`.
    template <typename TList, typename T>
    constexpr auto push_back_if_non_existant(TList l, T x)
    {
        return decltype(impl::push_back_if_non_existant_impl(l, x)){};
    }

    namespace impl
    {
        template <typename TL0, typename TL1>
        auto cat_unique_impl(TL0 l0, TL1 l1)
        {
            return mp::list::fold_l(l0,
                [](auto acc, auto x)
                {
                    return mp::list::push_back_if_non_existant(acc, x);
                },
                l1);
        }
    }

    /// @brief Concatenates `l0` and `l1` skipping duplicate elements.
    template <typename TL0, typename TL1>
    constexpr auto cat_unique(TL0 l0, TL1 l1)
    {
        return decltype(impl::cat_unique_impl(l0, l1)){};
    }
}
ECST_MP_LIST_NAMESPACE_END
