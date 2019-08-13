// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

#include "./impl/consumer_queue_ptr.hpp"
#include "./impl/pool.hpp"
#include "./impl/types.hpp"
#include "./impl/worker.hpp"

namespace ecst
{
    using thread_pool = etp::pool;
}
