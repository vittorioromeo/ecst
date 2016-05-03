// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./parameters.hpp"

ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE
{
    namespace split_evenly
    {
        template <                 // .
            typename TSubtaskCount // .
            >
        constexpr auto v(TSubtaskCount)
        {
            return impl::parameters< // .
                TSubtaskCount        // .
                >{};
        }
    }
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
