// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/debug.hpp>
#include <ecst/inner_par/utils.hpp>
#include <ecst/sig.hpp>

namespace ecst::inner_par::strategy::split_evenly_fn::impl
{
    template <typename TParameters>
    struct executor
    {
        using parameters = TParameters;

        template <typename TInstance, typename TContext, typename TF>
        void execute(TInstance& inst, TContext& ctx, TF&& f)
        {
            auto getter = parameters::subtask_count_getter();
            auto split_count = getter();
            auto per_split = inst.subscribed_count() / split_count;

            ECST_ASSERT_OP(inst.subscribed_count(), >=, split_count);

            ELOG(                                                   // .
                debug::lo_instance_parallelism()                    // .
                    << "split_evenly_fn:(" << inst.system_id()      // .
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
} // namespace ecst::inner_par::strategy::split_evenly_fn::impl
