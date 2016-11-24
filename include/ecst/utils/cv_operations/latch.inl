// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./cv_operations.hpp"
#include "./latch.hpp"

ECST_NAMESPACE
{
    latch::latch(
        impl::counter_inner_type initial_count) noexcept
        : _counter{initial_count}
    {
    }

    void latch::count_down() noexcept
    {
        --_counter;
    }

    void latch::decrement_and_notify_one() noexcept
    {
        impl::decrement_cv_counter_and_notify_one(_mutex, _cv, _counter);
    }

    void latch::decrement_and_notify_all() noexcept
    {
        impl::decrement_cv_counter_and_notify_all(_mutex, _cv, _counter);
    }

    template <typename TF>
    void latch::execute_and_wait_until_zero(TF && f) noexcept(
        noexcept(f()))
    {
        impl::execute_and_wait_until_counter_zero(
            _mutex, _cv, _counter, FWD(f));
    }

    void latch::wait() noexcept {
        impl::wait_counter(_mutex, _cv, _counter);
    }
}
ECST_NAMESPACE_END
