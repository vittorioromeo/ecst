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
    namespace split_evenly_fn
    {
        namespace impl
        {
            template <                       // .
                typename TSubtaskCountGetter // .
                >
            struct parameters
            {
                template <typename TParameters>
                using executor_type = executor<TParameters>;

                static constexpr auto subtask_count_getter()
                {
                    return TSubtaskCountGetter{};
                }
            };
        }
    }
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
