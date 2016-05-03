// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

ECST_NAMESPACE
{
    namespace this_thread = std::this_thread;
    namespace chrono = std::chrono;
    using std::thread;
    using std::mutex;
    using std::atomic;
    using std::condition_variable;
    using std::lock_guard;
    using std::unique_lock;
}
ECST_NAMESPACE_END
