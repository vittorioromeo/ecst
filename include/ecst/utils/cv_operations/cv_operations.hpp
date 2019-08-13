// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <utility>

namespace ecst::impl
{
    // Synchronization type aliases.
    using mutex_type = ecst::mutex;
    using cv_type = ecst::condition_variable;
    using lock_guard_type = ecst::lock_guard<mutex_type>;
    using unique_lock_type = ecst::unique_lock<mutex_type>;

    // In case the counter needs to be atomic in the future, an "inner type"
    // alias is defined.
    using counter_inner_type = sz_t;
    using counter_type = counter_inner_type;

    /// @brief Accesses `cv` and `c` through a `lock_guard` on `mutex`, and
    /// calls `f(cv, c)`.
    template <typename TF>
    inline void access_cv_counter(
        mutex_type& mutex, cv_type& cv, counter_type& c, TF&& f) noexcept
    {
        lock_guard_type l(mutex);
        f(cv, c);

        // Prevent warnings.
        (void)l;
    }

    /// @brief Decrements `c` through `mutex`, and calls `f(cv)`.
    template <typename TF>
    inline void decrement_cv_counter_then(
        mutex_type& mutex, cv_type& cv, counter_type& c, TF&& f) noexcept
    {
        access_cv_counter(mutex, cv, c, [&f](auto& y_cv, auto& x_c) {
            ECST_ASSERT(x_c > 0);
            --x_c;

            f(y_cv);
        });
    }

    /// @brief Decrements `c` through `mutex`, and calls `cv.notify_one()`.
    inline void decrement_cv_counter_and_notify_one(
        mutex_type& mutex, cv_type& cv, counter_type& c) noexcept
    {
        decrement_cv_counter_then(
            mutex, cv, c, [](auto& x_cv) { x_cv.notify_one(); });
    }

    /// @brief Decrements `c` through `mutex`, and calls `cv.notify_all()`.
    inline void decrement_cv_counter_and_notify_all(
        mutex_type& mutex, cv_type& cv, counter_type& c) noexcept
    {
        decrement_cv_counter_then(
            mutex, cv, c, [](auto& x_cv) { x_cv.notify_all(); });
    }

    /// @brief Executes `f`, locks `mutex`, and waits until `predicate`
    /// is `true` through `cv`.
    template <typename TPredicate, typename TF>
    inline void execute_and_wait_until(
        mutex_type& mutex, cv_type& cv, TPredicate&& predicate, TF&& f) noexcept
    {
        f();

        unique_lock_type l(mutex);
        cv.wait(l, FWD(predicate));
    }

    /// @brief Locks `mutex`, executes `f` and waits until `c` is zero
    /// through `cv`.
    template <typename TF>
    inline void execute_and_wait_until_counter_zero(
        mutex_type& mutex, cv_type& cv, counter_type& c, TF&& f) noexcept
    {
        execute_and_wait_until(
            mutex, cv, [&c] { return c == 0; }, FWD(f));
    }
} // namespace ecst::impl
