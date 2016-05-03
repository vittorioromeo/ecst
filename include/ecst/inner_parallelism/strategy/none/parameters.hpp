// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./executor.hpp"

ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE
{
    namespace none
    {
        namespace impl
        {
            struct parameters
            {
                template <typename TParameters>
                using executor_type = executor<TParameters>;
            };
        }
    }
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
