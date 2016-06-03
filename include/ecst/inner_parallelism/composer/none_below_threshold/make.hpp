// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "../fixed_threshold.hpp"
#include "../../strategy/none.hpp"

ECST_INNER_PARALLELISM_COMPOSER_NAMESPACE
{
    namespace none_below_threshold
    {
        template <typename TEntityThreshold, typename TStrategyGreater>
        constexpr auto v(TEntityThreshold, TStrategyGreater)
        {
            constexpr auto none_fallback = // .
                ecst::inner_parallelism::strategy::none::v();

            return fixed_threshold::v( // .
                TEntityThreshold{},    // .
                TStrategyGreater{},    // .
                none_fallback          // .
                );
        }
    }
}
ECST_INNER_PARALLELISM_COMPOSER_NAMESPACE_END
