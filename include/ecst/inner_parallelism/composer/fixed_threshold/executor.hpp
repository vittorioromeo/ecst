// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/inner_parallelism/types.hpp>

ECST_INNER_PARALLELISM_COMPOSER_NAMESPACE
{
    namespace fixed_threshold
    {
        namespace impl
        {
            template <typename TParameters>
            struct executor
            {
                using parameters = TParameters;
                using strategy_greater = typename parameters::strategy_greater;
                using strategy_lower = typename parameters::strategy_lower;

                using executor_greater = // .
                    inner_parallelism::executor_type<strategy_greater>;

                using executor_lower = // .
                    inner_parallelism::executor_type<strategy_lower>;

                executor_greater _s_greater;
                executor_lower _s_lower;

                template <typename TInstance, typename... Ts>
                void execute(TInstance& inst, Ts&&... xs)
                {
                    auto threshold_reached = // .
                        inst.subscribed_count() >=
                        parameters::entity_threshold();

                    if(threshold_reached)
                    {
                        _s_greater.execute(inst, FWD(xs)...);
                    }
                    else
                    {
                        _s_lower.execute(inst, FWD(xs)...);
                    }
                }
            };
        }
    }
}
ECST_INNER_PARALLELISM_COMPOSER_NAMESPACE_END
