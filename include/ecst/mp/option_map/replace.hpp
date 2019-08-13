// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list.hpp>

namespace ecst::mp::option_map::impl
{
    // From:
    // http://stackoverflow.com/questions/37386345

    template <typename NewPair>
    struct replace_helper_t
    {
        NewPair const& new_pair;

        template <typename Pair>
        constexpr decltype(auto) operator()(Pair&& p) const noexcept
        {
            return bh::if_(
                bh::equal(bh::first(new_pair), bh::first(p)), new_pair, FWD(p));
        }
    };

    struct replace_t
    {
        template <typename Map, typename NewPair>
        constexpr auto operator()(Map&& m, NewPair&& new_pair) const noexcept
        {
            return bh::unpack(FWD(m),
                bh::on(bh::make_map, replace_helper_t<NewPair>{FWD(new_pair)}));
        }
    };

    constexpr replace_t replace{};
} // namespace ecst::mp::option_map::impl
