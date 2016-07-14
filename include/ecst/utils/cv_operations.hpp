// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <utility>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    namespace impl
    {
        using mutex_type = ecst::mutex;
        using cv_type = ecst::condition_variable;
        using counter_type = sz_t;
        using lock_guard_type = ecst::lock_guard<mutex_type>;
        using unique_lock_type = ecst::unique_lock<mutex_type>;
    }

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

    namespace impl
    {
        /// @brief Accesses `cv` and `c` through a `lock_guard` on `mutex`, and
        /// calls `f(cv, c)`.
        template <typename TF>
        void access_cv_counter(
            mutex_type& mutex, cv_type& cv, counter_type& c, TF&& f) noexcept
        {
            lock_guard_type l(mutex);
            f(cv, c);

            // Prevent warnings.
            (void)l;
        }

        /// @brief Decrements `c` through `mutex`, and calls `f(cv)`.
        template <typename TF>
        void decrement_cv_counter_then(
            mutex_type& mutex, cv_type& cv, counter_type& c, TF&& f) noexcept
        {
            access_cv_counter(mutex, cv, c, [&f](auto& x_cv, auto& x_c)
                {
                    ECST_ASSERT_OP(x_c, >, 0);
                    --x_c;

                    f(x_cv);
                });
        }

        /// @brief Decrements `c` through `mutex`, and calls `cv.notify_one()`.
        void decrement_cv_counter_and_notify_one(
            mutex_type& mutex, cv_type& cv, counter_type& c) noexcept
        {
            decrement_cv_counter_then(mutex, cv, c, [](auto& x_cv)
                {
                    x_cv.notify_one();
                });
        }

        /// @brief Decrements `c` through `mutex`, and calls `cv.notify_all()`.
        void decrement_cv_counter_and_notify_all(
            mutex_type& mutex, cv_type& cv, counter_type& c) noexcept
        {
            decrement_cv_counter_then(mutex, cv, c, [](auto& x_cv)
                {
                    x_cv.notify_all();
                });
        }

        /// @brief Executes `f`, locks `mutex`, and waits until `predicate`
        /// is `true` through `cv`.
        template <typename TPredicate, typename TF>
        void execute_and_wait_until(mutex_type& mutex, cv_type& cv,
            TPredicate&& predicate, TF&& f) noexcept
        {
            f();

            unique_lock_type l(mutex);
            cv.wait(l, FWD(predicate));
        }

        /// @brief Locks `mutex`, executes `f` and waits until `c` is zero
        /// through `cv`.
        template <typename TF>
        void execute_and_wait_until_counter_zero(
            mutex_type& mutex, cv_type& cv, counter_type& c, TF&& f) noexcept
        {
            execute_and_wait_until(mutex, cv,
                [&c]
                {
                    return c == 0;
                },
                FWD(f));
        }
    }

    /// @brief Decrements `cb`'s counter by one, and calls `notify_one` on the
    /// inner condition variable.
    void counter_blocker::decrement_and_notify_one() noexcept
    {
        impl::decrement_cv_counter_and_notify_one(_mutex, _cv, _counter);
    }

    /// @brief Decrements `cb`'s counter by one, and calls `notify_all` on the
    /// inner condition variable.
    void counter_blocker::decrement_and_notify_all() noexcept
    {
        impl::decrement_cv_counter_and_notify_all(_mutex, _cv, _counter);
    }

    /// @brief Executes `f` and waits until the blocker's counter is zero. Uses
    /// the blocker's synchronization primitives.
    template <typename TF>
    void counter_blocker::execute_and_wait_until_zero(TF && f) noexcept(
        noexcept(f()))
    {
        impl::execute_and_wait_until_counter_zero(
            _mutex, _cv, _counter, FWD(f));
    }
}
ECST_NAMESPACE_END
