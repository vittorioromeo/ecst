// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/mp/mp.hpp>

#define ECST_IMPL_MP_MAP_NAMESPACE \
    ECST_MP_NAMESPACE              \
    {                              \
    namespace map

#define ECST_IMPL_MP_MAP_NAMESPACE_END \
    }                                  \
    ECST_MP_NAMESPACE_END

/// @macro Opens the `ecst::mp::map` namespace.
#define ECST_MP_MAP_NAMESPACE ECST_IMPL_MP_MAP_NAMESPACE

/// @macro Closes the `ecst::mp::map` namespace.
#define ECST_MP_MAP_NAMESPACE_END ECST_IMPL_MP_MAP_NAMESPACE_END
