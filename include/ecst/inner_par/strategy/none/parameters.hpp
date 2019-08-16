// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./executor.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::inner_par::strategy::none::impl
{
    struct parameters
    {
        template <typename TParameters>
        using executor_type = executor<TParameters>;
    };
} // namespace ecst::inner_par::strategy::none::impl
