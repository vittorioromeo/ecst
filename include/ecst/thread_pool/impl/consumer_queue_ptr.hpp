// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./types.hpp"
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>

namespace etp
{
    /// @brief Wraps a `task_queue` pointer and a consumer token.
    class consumer_queue_ptr
    {
    private:
        task_queue* _queue;
        task_queue_consumer_token _ctok;

    public:
        consumer_queue_ptr(task_queue& queue) noexcept
            : _queue{&queue}, _ctok{queue}
        {
        }

        // Prevent copies.
        consumer_queue_ptr(const consumer_queue_ptr&) = delete;
        consumer_queue_ptr& operator=(const consumer_queue_ptr&) = delete;

        consumer_queue_ptr(consumer_queue_ptr&& rhs) noexcept
            : _queue{rhs._queue}, _ctok{*_queue}
        {
        }

        consumer_queue_ptr& operator=(consumer_queue_ptr&& rhs) noexcept
        {
            _queue = rhs._queue;
            _ctok = task_queue_consumer_token(*_queue);

            return *this;
        }

        auto& ctok() noexcept
        {
            return _ctok;
        }

        auto operator-> () noexcept
        {
            ECST_ASSERT(_queue != nullptr);
            return _queue;
        }

        auto operator-> () const noexcept
        {
            ECST_ASSERT(_queue != nullptr);
            return _queue;
        }
    };
} // namespace etp
