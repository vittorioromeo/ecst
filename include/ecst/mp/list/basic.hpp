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

    // Concats `l0` and `l1`, after calling `bh::sort` and `bh::unique`.
    template <typename TL0, typename TL1>
    auto unique_cat(TL0 && l0, TL1 && l1)
    {
        using namespace mp;
        return bh::unique(bh::sort(bh::concat(l0, l1)));
    }

    // Returns the index of `x` in `l`.
    template <typename TList, typename T>
    constexpr auto index_of(TList&&, T && x) noexcept
    {
        using Pred = decltype(bh::equal.to(x));
        using Pack = typename bh::detail::make_pack<TList>::type;
        return bh::size_c<bh::detail::index_if<Pred, Pack>::value>;
    }
}
ECST_MP_LIST_NAMESPACE_END
