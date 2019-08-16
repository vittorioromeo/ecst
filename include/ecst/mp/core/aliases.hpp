// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::mp
{
    using ecst::sz_t;
    using ecst::sz_t_;

    template <std::size_t TX>
    inline constexpr bh::size_t<TX> sz_v{};

    using ecst::int_;
    using ecst::int_v;
} // namespace ecst::mp
