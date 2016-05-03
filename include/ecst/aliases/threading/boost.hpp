// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

#pragma GCC system_header
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/chrono.hpp>

ECST_NAMESPACE
{
    namespace this_thread = boost::this_thread;
    namespace chrono = boost::chrono;
    using boost::thread;
    using boost::mutex;
    using boost::atomic;
    using boost::condition_variable;
    using boost::lock_guard;
    using boost::unique_lock;
}
ECST_NAMESPACE_END
