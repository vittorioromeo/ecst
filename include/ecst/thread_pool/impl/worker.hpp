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
    /// @brief Wraps an `std::thread` and atomic control variables.
    class worker
    {
    private:
        ecst::thread _thread;
        consumer_queue_ptr _queue;
        movable_atomic<bool> _running{false};
        movable_atomic<bool> _finished{false};

        void run()
        {
            ECST_ASSERT_NS(_running);

            // Next task buffer.
            task t;

            // While the worker is running...
            while(_running)
            {
                // ...dequeue a task (blocking).
                _queue->wait_dequeue(_queue.ctok(), t);

                // Execute the task.
                t();
            }

            // Signal the thread pool to send dummy final tasks.
            _finished = true;
        }

    public:
        worker(task_queue& queue) noexcept : _queue{queue}
        {
        }

        ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(worker);

        template <typename TCounter>
        void start(TCounter& remaining_inits)
        {
            ECST_ASSERT_NS(!_running);

            // Start the worker thread.
            _thread = ecst::thread([this, &remaining_inits]
                {
                    // Set the running flag and signal the pool the thread has
                    // been initialized.
                    _running = true;
                    (--remaining_inits);

                    run();
                });
        }

        /// @brief Sets the running flag to false, preventing the worker to
        /// accept tasks.
        void stop() noexcept
        {
            ECST_ASSERT_NS(_running);
            _running = false;
        }

        /// @brief Assuming the worker is not running, tries to join the
        /// underlying thread.
        void join() noexcept
        {
            ECST_ASSERT(_thread.joinable());
            ECST_ASSERT_NS(!_running);
            _thread.join();
        }

        /// @brief Returns `true` if the worker has exited the processing loop.
        auto finished() const noexcept
        {
            return _finished.load();
        }
    };
}
