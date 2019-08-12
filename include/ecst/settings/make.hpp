// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"
#include <ecst/config.hpp>

ECST_SETTINGS_NAMESPACE
{
    /// @brief Creates default settings for an `ecst::context`.
    constexpr auto make();
}
ECST_SETTINGS_NAMESPACE_END
