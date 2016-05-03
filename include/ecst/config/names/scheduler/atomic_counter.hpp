// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/scheduler/scheduler.hpp>

#define ECST_IMPL_SCHEDULER_ATOMIC_COUNTER_NAMESPACE \
    ECST_SCHEDULER_NAMESPACE                         \
    {                                                \
    namespace atomic_counter

#define ECST_IMPL_SCHEDULER_ATOMIC_COUNTER_NAMESPACE_END \
    }                                                    \
    ECST_SCHEDULER_NAMESPACE_END

/// @macro Opens the `ecst::scheduler::atomic_counter` namespace.
#define ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE \
    ECST_IMPL_SCHEDULER_ATOMIC_COUNTER_NAMESPACE

/// @macro Closes the `ecst::scheduler::atomic_counter` namespace.
#define ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE_END \
    ECST_IMPL_SCHEDULER_ATOMIC_COUNTER_NAMESPACE_END
