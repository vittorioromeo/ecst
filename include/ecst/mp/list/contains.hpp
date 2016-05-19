// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/basic.hpp>

ECST_MP_LIST_NAMESPACE
{

    /// @brief Returns true if any element in `l` is equal to `T`.
    template <typename TList, typename T>
    constexpr auto contains(TList l, T x)
    {
        return bh::contains(l, x);
    }
}
ECST_MP_LIST_NAMESPACE_END
