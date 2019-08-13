// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <vrm/core/strong_typedef.hpp>

namespace ecst
{
    VRM_CORE_STRONG_TYPEDEF(sz_t, entity_id);

    namespace impl
    {
        VRM_CORE_STRONG_TYPEDEF(sz_t, counter);
    }
} // namespace ecst
