// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include "./types.hpp"

namespace etp
{
    /// @brief Wraps a `task_queue` pointer and a consumer token.
    class producer_queue_uptr
    {
    private:
        std::unique_ptr<task_queue> _queue;
        task_queue_producer_token _ptok;

    public:
        producer_queue_uptr() noexcept : _queue{std::make_unique<task_queue>()},
                                         _ptok{*_queue}
        {
        }

        // Prevent copies.
        producer_queue_uptr(const producer_queue_uptr&) = delete;
        producer_queue_uptr& operator=(const producer_queue_uptr&) = delete;

        producer_queue_uptr(producer_queue_uptr&& rhs) noexcept
            : _queue{std::move(rhs._queue)},
              _ptok{*_queue}
        {
        }

        producer_queue_uptr& operator=(producer_queue_uptr&& rhs) noexcept
        {
            _queue = std::move(rhs._queue);
            _ptok = task_queue_producer_token(*_queue);

            return *this;
        }

        auto& ptok() noexcept
        {
            return _ptok;
        }

        auto& queue() noexcept
        {
            ECST_ASSERT(_queue != nullptr);
            return *_queue;
        }

        auto operator-> () noexcept
        {
            ECST_ASSERT(_queue != nullptr);
            return _queue.get();
        }

        auto operator-> () const noexcept
        {
            ECST_ASSERT(_queue != nullptr);
            return _queue.get();
        }
    };
}
