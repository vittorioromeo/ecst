// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core/aliases.hpp>
#include <type_traits>

namespace ecst::mp
{
    inline constexpr auto same_type = [](auto a, auto b) {
        return std::is_same<decltype(a), decltype(b)>{};
    };

    inline constexpr auto same_type_decay = [](auto a, auto b) {
        return std::is_same<std::decay_t<decltype(a)>,
            std::decay_t<decltype(b)>>{};
    };
} // namespace ecst::mp
