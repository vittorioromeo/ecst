// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list/types.hpp>

namespace ecst::mp::list
{
    // Returns a list created from the passed argument types.
    template <typename... Ts>
    constexpr auto make(Ts...)
    {
        return v<Ts...>;
    }

    // Executes `bh::all` on all `xs...`.
    template <typename... Ts>
    constexpr auto all_variadic(Ts&&... xs)
    {
        return bh::all(bh::make_basic_tuple(FWD(xs)...));
    }

    // Returns the index of `x` in `l`.
    template <typename TList, typename T>
    constexpr auto index_of(TList l, T&& x) noexcept
    {
        auto size = decltype(bh::size(l)){};
        auto dropped =
            decltype(bh::size(bh::drop_while(l, bh::not_equal.to(x)))){};

        return size - dropped;
    }

    // Right-folds over a sequence, also passing the current index.
    template <typename TSeq, typename TAcc, typename TF>
    constexpr auto indexed_fold_right(TSeq&& seq, TAcc&& acc, TF&& f) noexcept
    {
        return bh::second(bh::fold_right(FWD(seq),
            bh::make_pair(bh::size(seq) - sz_v<1>, FWD(acc)),
            [&f](auto&& x, auto&& idx_acc_pair) {
                auto curr_idx = bh::first(FWD(idx_acc_pair));
                auto curr_acc = bh::second(FWD(idx_acc_pair));

                auto next_acc = f(FWD(x), curr_acc, curr_idx);
                return bh::make_pair(curr_idx - sz_v<1>, next_acc);
            }));
    }

    template <typename TSeq, typename T, typename TCondition>
    constexpr auto append_if(TSeq&& seq, T&& x, TCondition&& c) noexcept
    {
        if constexpr(c)
        {
            return bh::append(FWD(seq), FWD(x));
        }
        else
        {
            return seq;
        }
    }

    // Returns the index of the first element `x` of `t` satisfying `p(x)`.
    template <typename TTuple, typename TPredicate>
    constexpr auto index_of_first_matching(TTuple&& t, TPredicate&& p) noexcept
    {
        auto res = indexed_fold_right(
            FWD(t), empty_v, [&p](auto&& x, auto acc, auto idx) {
                return append_if(acc, idx, p(x));
            });

        return bh::front(res);
    }

    // Converts two lists to sets and returns their intersection.
    template <typename TL0, typename TL1>
    constexpr auto list_to_set_intersection(TL0&& l0, TL1&& l1) noexcept
    {
        return bh::intersection(bh::to_set(FWD(l0)), bh::to_set(FWD(l1)));
    }

    // Returns true if `l0` and `l1` have at least one element in common.
    template <typename TL0, typename TL1>
    constexpr auto any_common_element(TL0&& l0, TL1&& l1) noexcept
    {
        return list_to_set_intersection(FWD(l0), FWD(l1)) != bh::make_set();
    }
} // namespace ecst::mp::list
