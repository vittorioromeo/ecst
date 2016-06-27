// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include "./types.hpp"
#include "./consumer_queue_ptr.hpp"

namespace etp
{
    /// @brief Wraps an `std::thread`, `consumer_queue_ptr` and atomic control
    /// variables.
    class worker
    {
    private:
        enum class state
        {
            // Initial state of the `worker`.
            uninitialized,

            // The `worker` is dequeuing and accepting tasks.
            running,

            // The `worker` will not dequeue tasks anymore and will try to
            // transition to `state::finished` automatically.
            stopped,

            // The `worker` is done. The thread can be joined.
            finished
        };

        ecst::thread _thread;
        consumer_queue_ptr _queue;
        ecst::movable_atomic<state> _state{state::uninitialized};

        void run()
        {
            ECST_ASSERT_NS(_state == state::running);

            // Next task buffer.
            task t;

            // While the worker is running...
            while(_state == state::running)
            {
                // ...dequeue a task (blocking).
                _queue->wait_dequeue(_queue.ctok(), t);

                // Execute the task.
                t();
            }

            // Signal the thread pool to send dummy final tasks.
            _state = state::finished;
        }

    public:
        worker(task_queue& queue) noexcept : _queue{queue}
        {
        }

        ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(worker);

        template <typename TCounter>
        void start(TCounter& remaining_inits)
        {
            ECST_ASSERT_NS(_state == state::uninitialized);

            // Start the worker thread.
            _thread = ecst::thread([this, &remaining_inits]
                {
                    // Set the running flag and signal the pool the thread has
                    // been initialized.
                    _state = state::running;
                    (--remaining_inits);

                    run();
                });
        }

        /// @brief Sets the running flag to false, preventing the worker to
        /// accept tasks.
        void stop() noexcept
        {
            ECST_ASSERT_NS(_state == state::running);
            _state = state::stopped;
        }

        /// @brief Assuming the worker is not running, tries to join the
        /// underlying thread.
        void join() noexcept
        {
            ECST_ASSERT(_thread.joinable());
            ECST_ASSERT_NS(_state == state::finished);
            _thread.join();
        }

        /// @brief Returns `true` if the worker has exited the processing loop.
        auto finished() const noexcept
        {
            return _state == state::finished;
        }
    };
}
