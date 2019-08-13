// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./cv_operations.hpp"

namespace ecst
{
    /// @brief Data structure containing synchronization primitives required for
    /// a "blocking counter" with an `std::condition_variable` and `std::mutex`.
    class latch
    {
    private:
        impl::cv_type _cv;
        impl::mutex_type _mutex;
        impl::counter_type _counter;

    public:
        /// @brief Constructs a latch with `initial_count` counter value.
        latch(impl::counter_inner_type initial_count) noexcept;

        /// @brief Decrements the counter and notifies one thread waiting on the
        /// `condition_variable`.
        /// @details Asserts `_counter` to be greater than zero.
        void decrement_and_notify_one() noexcept;

        /// @brief Decrements the counter and notifies all threads waiting on
        /// the `condition_variable`.
        /// @details Asserts `_counter` to be greater than zero.
        void decrement_and_notify_all() noexcept;

        /// @brief Immediately executes `f` and blocks the caller thread until
        /// the counter reaches zero.
        /// @details Assumes that `f` will trigger a chain of operations that
        /// will decrement the counter to zero.
        template <typename TF>
        void execute_and_wait_until_zero(TF&& f) noexcept(noexcept(f()));
    };
} // namespace ecst
