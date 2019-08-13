// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp.hpp>

namespace ecst::inner_parallelism
{
    namespace utils
    {
        template <typename TFRIST, typename TF>
        void execute_split_runtime(                       // .
            sz_t total, sz_t per_split, sz_t split_count, // .
            TFRIST& rist, TF&& f)                         // .
            noexcept(noexcept(                            // .
                f(0, 0, 0)                                // .
                ))
        {
            ECST_ASSERT_OP(split_count, >, 0);

            // Build and run additional subtasks.
            // `f` is called through `rist` in order to forward the additional
            // subtasks to the thread pool.
            for(sz_t i_split = 0; i_split < split_count - 1; ++i_split)
            {
                auto i_begin = i_split * per_split;
                auto i_end = (i_split + 1) * per_split;
                rist(f)(i_split, i_begin, i_end);
            }

            // Builds and runs the last subtask.
            // `f` is called directly as it will be executed in the caller
            // thread.
            {
                auto i_split = split_count - 1;
                auto i_begin = i_split * per_split;
                auto i_end = total;
                f(i_split, i_begin, i_end);
            }
        }

        template <typename TInstance, typename TContext, typename TF>
        void prepare_execute_wait_subtasks(   // .
            TInstance& inst, TContext& ctx,   // .
            sz_t split_count, sz_t per_split, // .
            TF&& f)
        {
            // Assert there is at least one split.
            ECST_ASSERT_OP(split_count, >, 0);

            // Bind the `rist` (run in separate thread) adapter to the execution
            // function, which will generate entity subranges.
            auto ef = [&](auto& rist) mutable {
                // Builds and runs the subtasks.
                utils::execute_split_runtime(
                    inst.subscribed_count(), per_split, split_count, rist, f);
            };

            // Prepare states, run subtasks and block until done.
            inst.prepare_and_wait_subtasks(ctx, split_count, ef);
        }
    } // namespace utils
} // namespace ecst::inner_parallelism
