// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <tuple>
#include <utility>
#include <ecst/config.hpp>
#include <ecst/mp/list/replace_at.hpp>
#include <ecst/mp/list/index_list.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TAcc, typename TF, sz_t... TIs, typename TListList>
        auto fold_l_with_index_impl_variadic(
            TAcc acc, TF f, std::index_sequence<TIs...>, TListList ll)
        {
            // Folding step.
            auto step = [=](auto self, auto y_curr, auto yi, auto... yis)
            {
                // Compute next folding step.
                auto next_acc(f(yi, y_curr, at(std::get<TIs>(ll), yi)...));

                // Check if there are any more items inside the list.
                return static_if(bool_v<(sizeof...(yis) > 0)>)
                    .then([=](auto z_self)
                        {
                            // Recursive case.
                            return z_self(next_acc, yis...);
                        })
                    .else_([=](auto)
                        {
                            // Base case.
                            return next_acc;
                        })(self);
            };

            // Minimum index sequence list for all passed lists.
            auto mil(make_minimal_index_list_for_by_unpacking(ll));

            // It is necessary to explicitly check for an empty list.
            return static_if(empty(mil))
                .then([=](auto)
                    {
                        return acc;
                    })
                .else_([=](auto)
                    {
                        // Unpack the list `l` into a call to `step`.
                        return apply(
                            [=](auto... xis)
                            {
                                return y_combinator(step)(acc, xis...);
                            },
                            mil);
                    })(ll);
        }

        template <typename TBase, typename TF, typename... TLists>
        auto fold_l_with_index_x_impl(TBase base, TF f, TLists... ls)
        {
            return fold_l_with_index_impl_variadic(base, f,
                std::make_index_sequence<sizeof...(ls)>(), make(ls...));
        }

        template <typename TBase, typename TF, typename... TLists>
        auto fold_l_with_index_impl(TBase base, TF f, TLists... ls)
        {
            return fold_l_with_index_x_impl(base, f, ls...);
        }
    }

    /// @brief Executes a left-fold on all the lists `ls...`.
    /// @details Keeps track of the index.
    template <typename TBase, typename TF, typename... TLists>
    constexpr auto fold_l_with_index(TBase base, TF f, TLists... ls)
    {
        return decltype(impl::fold_l_with_index_impl(base, f, ls...)){};
    }

    /// @brief Executes a left-fold on all the lists in `ll`.
    /// @details Keeps track of the index.
    template <typename TBase, typename TF, typename TListList>
    constexpr auto fold_l_with_index_by_unpacking(
        TBase base, TF f, TListList ll)
    {
        return apply(
            [=](auto... ls)
            {
                return fold_l_with_index(base, f, ls...);
            },
            ll);
    }

    namespace impl
    {
        template <typename TBase, typename TF, typename... TLists>
        constexpr auto fold_l_impl(TBase base, TF f, TLists... ls)
        {
            return fold_l_with_index(base,
                [f](auto, auto x_xl, auto... x_xs)
                {
                    return f(x_xl, x_xs...);
                },
                ls...);
        }
    }

    /// @brief Executes a left-fold on all the lists `ls...`.
    template <typename TBase, typename TF, typename... TLists>
    constexpr auto fold_l(TBase base, TF f, TLists... ls)
    {
        return decltype(impl::fold_l_impl(base, f, ls...)){};
    }

    /// @brief Executes a left-fold on all the lists `ll`.
    template <typename TBase, typename TF, typename TListList>
    constexpr auto fold_l_by_unpacking(TBase base, TF f, TListList ll)
    {
        return apply(
            [=](auto... ls)
            {
                return fold_l(base, f, ls...);
            },
            ll);
    }
}
ECST_MP_LIST_NAMESPACE_END
