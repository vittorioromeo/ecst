// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/mp/mp.hpp>

#define ECST_IMPL_MP_PAIR_NAMESPACE \
    ECST_MP_NAMESPACE               \
    {                               \
    namespace pair

#define ECST_IMPL_MP_PAIR_NAMESPACE_END \
    }                                   \
    ECST_MP_NAMESPACE_END

/// @macro Opens the `ecst::mp::pair` namespace.
#define ECST_MP_PAIR_NAMESPACE ECST_IMPL_MP_PAIR_NAMESPACE

/// @macro Closes the `ecst::mp::pair` namespace.
#define ECST_MP_PAIR_NAMESPACE_END ECST_IMPL_MP_PAIR_NAMESPACE_END
