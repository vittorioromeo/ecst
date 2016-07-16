// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./cv_operations.hpp"

ECST_NAMESPACE
{
    /// @brief Data structure containing synchronization primitives required for
    /// a "blocking counter" with an `std::condition_variable` and `std::mutex`.
    class counter_blocker
    {
    private:
        impl::cv_type _cv;
        impl::mutex_type _mutex;
        impl::counter_type _counter;

    public:
        counter_blocker(impl::counter_type initial_count) noexcept
            : _counter{initial_count}
        {
        }

        void decrement_and_notify_one() noexcept;
        void decrement_and_notify_all() noexcept;

        template <typename TF>
        void execute_and_wait_until_zero(TF&& f) noexcept(noexcept(f()));
    };
}
ECST_NAMESPACE_END
