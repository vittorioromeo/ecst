// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <bitset>
#include <ecst/config.hpp>
#include <ecst/aliases/vrmc.hpp>

ECST_NAMESPACE
{
    namespace impl
    {
        template <sz_t TN>
        using bitset = std::bitset<TN>;
    }
}
ECST_NAMESPACE_END
