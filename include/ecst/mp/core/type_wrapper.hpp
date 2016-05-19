// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core/aliases.hpp>

ECST_MP_NAMESPACE
{
    using bh::type_c;

    template <typename T>
    using unwrap = typename T::type;
}
ECST_MP_NAMESPACE_END
