// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <bitset>
#include <ecst/aliases/vrmc.hpp>
#include <ecst/config.hpp>

ECST_NAMESPACE
{
    namespace impl
    {
        /// @brief Dense bitset.
        template <sz_t TN>
        using dense_bitset = std::bitset<TN>;
    } // namespace impl
}
ECST_NAMESPACE_END
