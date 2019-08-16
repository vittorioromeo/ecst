// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"
#include <ecst/config.hpp>

namespace ecst::settings
{
    /// @brief Creates default settings for an `ecst::ctx`.
    constexpr auto make();
} // namespace ecst::settings
