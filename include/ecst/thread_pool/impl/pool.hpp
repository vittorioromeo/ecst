// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vector>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/hardware.hpp>
#include "./types.hpp"
#include "./worker.hpp"

namespace etp
{
    class pool
    {
    private:
        task_queue _queue;
        std::vector<worker> _workers;
        std::atomic<sz_t> _remaining_inits;

        /// @brief Returns `true` if all workers have finished (exited from
        /// loop).
        auto all_workers_finished() const noexcept
        {
            for(const auto& w : _workers)
            {
                if(!w.finished())
                {
                    return false;
                }
            }

            return true;
        }

        /// @brief Posts a dummy empty task, used to unblock workers waiting to
        /// be destroyed.
        void post_dummy_task()
        {
            post([]
                {
                });
        }

        /// @brief Creates and starts `n` workers, also initializing the
        /// atomic remaining inits counter.
        void initialize_workers(sz_t n)
        {
            // Create workers.
            for(decltype(n) i(0); i < n; ++i)
            {
                _workers.emplace_back(_queue);
            }

            // Start workers.
            _remaining_inits = n;
            for(auto& w : _workers)
            {
                w.start(_remaining_inits);
            }
        }

    public:
        pool()
        {
            auto hwc = ecst::hardware::status::core_count();
            auto worker_count = hwc;
            initialize_workers(worker_count);
        }

        ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(pool);

        ~pool()
        {
            // Busy wait for all workers to be initialized.
            while(_remaining_inits > 0)
            {
                ecst::this_thread::sleep_for(ecst::chrono::milliseconds(1));
            }

            // Signal all workers to exit their processing loops.
            for(auto& w : _workers)
            {
                w.stop();
            }

            // Post dummy tasks until all workers have exited their loops.
            while(!all_workers_finished())
            {
                post_dummy_task();
            }

            // Join the workers' threads.
            for(auto& w : _workers)
            {
                w.join();
            }
        }

        template <typename TF>
        void post(TF&& f)
        {
            _queue.enqueue(std::move(f));
        }
    };
}
