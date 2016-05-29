// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list/types.hpp>

ECST_MP_LIST_NAMESPACE
{
    // Returns a list created from the passed argument types.
    template <typename... Ts>
    constexpr auto make(Ts...)
    {
        return v<Ts...>;
    }

    // Executes `bh::all` on all `xs...`.
    template <typename... Ts>
    constexpr auto all_variadic(Ts && ... xs)
    {
        return bh::all(bh::make_basic_tuple(FWD(xs)...));
    }

    // TODO: please
    template <typename TTuple, typename TPredicate>
    constexpr auto index_of_first_matching(
        TTuple && t, TPredicate && p) noexcept
    {
        auto res = bh::fold_left(t, bh::make_pair(sz_v<0>, bh::false_c),
            [](auto&& acc, auto&& x)
            {
                return static_if(bh::second(acc))
                    .then([](auto&& xacc, auto&&)
                        {
                            return xacc;
                        })
                    .else_([](auto&& xacc, auto&& xx)
                        {
                            return static_if(bh::bool_c<           // .
                                                 decltype(p(xx)){} // .
                                                 >)
                                .then([](auto&& yacc)
                                    {
                                        return bh::make_pair(
                                            bh::first(decltype(yacc){}),
                                            bh::true_c);
                                    })
                                .else_([](auto&& yacc)
                                    {
                                        return bh::make_pair(
                                            sz_v<bh::first(decltype(yacc){}) +
                                                 sz_v<1>>,
                                            bh::false_c);

                                    })(FWD(xacc));
                        })(FWD(acc), FWD(x));
            });

        return bh::first(decltype(res){});
    }

    // Returns the index of `x` in `l`.
    template <typename TList, typename T>
    constexpr auto index_of(TList&&, T && x) noexcept
    {
        using Pred = decltype(bh::equal.to(x));
        using Pack = typename bh::detail::make_pack<TList>::type;
        return bh::size_c<bh::detail::index_if<Pred, Pack>::value>;
    }

    // Converts two lists to sets and returns their intersection.
    template <typename TL0, typename TL1>
    constexpr auto list_to_set_intersection(TL0 && l0, TL1 && l1) noexcept
    {
        return bh::intersection(bh::to_set(FWD(l0)), bh::to_set(FWD(l1)));
    }

    // Returns true if `l0` and `l1` have at least one element in common.
    template <typename TL0, typename TL1>
    constexpr auto any_common_element(TL0 && l0, TL1 && l1) noexcept
    {
        return list_to_set_intersection(FWD(l0), FWD(l1)) != bh::make_set();
    }
}
ECST_MP_LIST_NAMESPACE_END
