// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./executor.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

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
        } // namespace impl
    }     // namespace split_evenly_fn
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
