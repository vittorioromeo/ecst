// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

#include "./impl/types.hpp"
#include "./impl/producer_queue_uptr.hpp"
#include "./impl/consumer_queue_ptr.hpp"
#include "./impl/worker.hpp"
#include "./impl/pool.hpp"

ECST_NAMESPACE
{
    using thread_pool = etp::pool;
}
ECST_NAMESPACE_END
