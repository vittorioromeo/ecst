// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/fold.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        /// @brief Executes a left fold on `l`, which returns all items that
        /// satisfy `c`.
        /// @details Used to implement `filter`.
        template <typename TList, typename TComparer>
        constexpr auto fold_l_cat_matching(TList l, TComparer c)
        {
            (void)c;

            return list::fold_l_with_index(empty_v,
                [=](auto i, auto xl, auto x)
                {
                    // Pass both index and current value to comparer.
                    return static_if(bool_v<decltype(c(i, xl, x)){}>)
                        .then([](auto ayi, auto yl)
                            {
                                return append(yl, ayi);
                            })
                        .else_([](auto, auto yl)
                            {
                                return yl;
                            })(i, xl);
                },
                l);
        }

        /// @brief Returns a list of the indices of the indices in `l` that
        /// satisfy `c`.
        template <typename TList, typename TComparer>
        constexpr auto find_indices_of_matching_indices_impl(
            TList l, TComparer c)
        {
            // Fold and cat matching indices.
            return fold_l_cat_matching(l, [=](auto i, auto, auto)
                {
                    return c(i);
                });
        }

        /// @brief Returns a list of the indices of the elements in `l` that
        /// satisfy `c`.
        template <typename TList, typename TComparer>
        constexpr auto find_indices_of_matching_impl(TList l, TComparer c)
        {
            // Fold and cat matching items.
            return fold_l_cat_matching(l, [=](auto, auto, auto ax)
                {
                    return c(ax);
                });
        }
    }

    /// @brief Returns a list of the indices of the indices in `l` that satisfy
    /// `c`.
    template <typename TList, typename TComparer>
    constexpr auto find_indices_of_matching_indices(TList l, TComparer c)
    {
        return decltype(impl::find_indices_of_matching_indices_impl(l, c)){};
    }

    /// @brief Returns a list of the indices of the elements in `l` that satisfy
    /// `c`.
    template <typename TList, typename TComparer>
    constexpr auto find_indices_of_matching(TList l, TComparer c)
    {
        return decltype(impl::find_indices_of_matching_impl(l, c)){};
    }

    /// @brief Returns a list of the elements in `l` that satisfy `c`.
    template <typename TList, typename TComparer>
    constexpr auto find_matching(TList l, TComparer c)
    {
        return pick_at_by_unpacking(l, find_indices_of_matching(l, c));
    }

    /// @brief Given a list and a function, applies the function on the list if
    /// non-empty, otherwise returns `null_v`.
    template <typename TList, typename TF>
    constexpr auto null_if_empty_or_f(TList l, TF f)
    {
        return static_if(bool_v<list::empty(TList{})>)
            .then([](auto)
                {
                    return null_v;
                })
            .else_([=](auto xxl)
                {
                    return f(xxl);
                })(l);
    }

    /// @brief Given a list, returns its head if non-empty, otherwise `null_v`.
    template <typename TList>
    constexpr auto null_if_empty_or_head(TList l)
    {
        return null_if_empty_or_f(l, [](auto xl)
            {
                return head(xl);
            });
    }


    namespace impl
    {
        template <typename TList, typename TComparer>
        constexpr auto find_first_index_of_matching_impl(TList l, TComparer c)
        {
            auto res = find_indices_of_matching(l, c);
            return null_if_empty_or_head(res);
        }
    }

    /// @brief Returns the index of the first element in `l` that satisfies `c`.
    /// @details Returns `null_v` if there is no such element.
    template <typename TList, typename TComparer>
    constexpr auto find_first_index_of_matching(TList l, TComparer c)
    {
        return decltype(impl::find_first_index_of_matching_impl(l, c)){};
    }

    /// @brief Returns a list of the indices of elements of `l` equal to `T`.
    template <typename TList, typename T>
    constexpr auto find_indices_of(TList l, T)
    {
        return find_indices_of_matching(l, equal_to<T>);
    }

    /// @brief Returns the index of the first element of `l` equal to `T`
    /// @details Returns `null_v` if there is no such element.
    template <typename TList, typename T>
    constexpr auto find_first_index_of(TList l, T)
    {
        return find_first_index_of_matching(l, equal_to<T>);
    }

    namespace impl
    {
        template <typename TList, typename TComparer>
        constexpr auto find_if_impl(TList l, TComparer c)
        {
            auto res = find_first_index_of_matching(l, c);
            return static_if(is_null(res))
                .then([](auto)
                    {
                        return null_v;
                    })
                .else_([=](auto y)
                    {
                        return at(l, y);
                    })(res);
        }
    }

    /// @brief Returns the first element in `l` that satisfies `c`.
    /// @details Returns `null_v` if there is no such element.
    template <typename TList, typename TComparer>
    constexpr auto find_if(TList l, TComparer c)
    {
        return decltype(impl::find_if_impl(l, c)){};
    }

    /// @brief Returns the first element in `l` that satisfies `c`.
    /// @details Alias of `find_if`.
    template <typename TList, typename TComparer>
    constexpr auto find_first_matching(TList l, TComparer c)
    {
        return find_if(l, c);
    }

    /// @brief Returns the first element in `l` equal to `T`.
    /// @details Returns `null_v` if there is no such element.
    template <typename TList, typename T>
    constexpr auto find(TList l, T)
    {
        return find_if(l, equal_to<T>);
    }
}
ECST_MP_LIST_NAMESPACE_END
