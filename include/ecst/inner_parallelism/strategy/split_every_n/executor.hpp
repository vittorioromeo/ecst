// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/signature.hpp>
#include <ecst/debug.hpp>
#include <ecst/utils/cv_operations.hpp>
#include <ecst/inner_parallelism/utils.hpp>

ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE
{
    namespace split_every_n
    {
        namespace impl
        {
            template <typename TParameters>
            struct executor
            {
                using parameters = TParameters;

                template <typename TInstance, typename TContext, typename TF>
                void execute(TInstance& inst, TContext& ctx, TF&& f)
                {
                    constexpr auto per_split = // .
                        parameters::entities_per_subtask();

                    auto split_count = inst.subscribed_count() / per_split;

                    ELOG(                                                   // .
                        debug::lo_instance_parallelism()                    // .
                            << "split_every_n:(" << inst.system_id()        // .
                            << "):\n\tsubscribed_count()="                  // .
                            << inst.subscribed_count()                      // .
                            << "\n\tper_split=" << per_split                // .
                            << "\n\tsplit_count=" << split_count << "\n\n"; // .
                        );

                    // Executes all subtasks. Blocks until completed.
                    utils::prepare_execute_wait_subtasks( // .
                        inst, ctx, split_count, per_split, f);
                }
            };
        }
    }
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
