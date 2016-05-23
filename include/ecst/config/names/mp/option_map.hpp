// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./mp.hpp"

#define ECST_IMPL_MP_OPTION_MAP_NAMESPACE \
    ECST_MP_NAMESPACE                     \
    {                                     \
    namespace option_map

#define ECST_IMPL_MP_OPTION_MAP_NAMESPACE_END \
    }                                         \
    ECST_MP_NAMESPACE_END

/// @macro Opens the `ecst::mp::option_map` namespace.
#define ECST_MP_OPTION_MAP_NAMESPACE ECST_IMPL_MP_OPTION_MAP_NAMESPACE

/// @macro Closes the `ecst::mp::option_map` namespace.
#define ECST_MP_OPTION_MAP_NAMESPACE_END ECST_IMPL_MP_OPTION_MAP_NAMESPACE_END
