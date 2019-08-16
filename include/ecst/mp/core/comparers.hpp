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
    inline constexpr auto same_type = []<typename T0, typename T1>(T0, T1)
    {
        return std::is_same<T0, T1>{};
    };

    inline constexpr auto same_type_decay = []<typename T0, typename T1>(T0, T1)
    {
        return std::is_same<std::decay_t<T0>, std::decay_t<T1>>{};
    };
} // namespace ecst::mp
