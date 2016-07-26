// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./task_dependency_data.hpp"
#include "./task.hpp"
#include "./task_group.hpp"

ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE
{
    template <typename TDependencyData>
    template <                    // .
        typename TTaskGroup,      // .
        typename TCounterBlocker, // .
        typename TID,             // .
        typename TContext,        // .
        typename TF               // .
        >
    void task<TDependencyData>::run( // .
        TTaskGroup & tg,             // .
        TCounterBlocker & b,         // .
        TID my_id,                   // .
        TContext & ctx,              // .
        TF & f                       // .
        )
    {
        // Get system instance from task ID.
        auto& s_instance(ctx.instance_by_id(my_id));

        // Execute processing function on system context.
        s_instance.execute(ctx, f);

        // Safely decrement remaining tasks counter.
        b.decrement_and_notify_one();

        // For every dependent task ID...
        dependency_data().for_dependent_ids([this, &tg, &b, &ctx, &f](auto id)
            {
                // ...retrieve the corresponding task.
                auto& dt = tg.task_by_id(id);

                // Then, inform the task that one of its dependencies (the
                // current task) has been executed.
                if(!dt.dependency_data().decrement_and_check())
                {
                    // Exit if dependencies still need to be processed.
                    return;
                }

                // Recursively run the dependent instance.
                // Use of multithreading:
                // * Run children instance tasks in separate thread.
                ctx.post_in_thread_pool([this, &dt, &tg, &b, &ctx, &f, id]
                    {
                        dt.run(tg, b, id, ctx, f);
                    });
            });
    }
}
ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE_END
