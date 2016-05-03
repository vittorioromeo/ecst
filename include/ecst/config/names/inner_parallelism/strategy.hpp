// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/inner_parallelism/inner_parallelism.hpp>

#define ECST_IMPL_INNER_PARALLELISM_STRATEGY_NAMESPACE \
    ECST_INNER_PARALLELISM_NAMESPACE                   \
    {                                                  \
    namespace strategy

#define ECST_IMPL_INNER_PARALLELISM_STRATEGY_NAMESPACE_END \
    }                                                      \
    ECST_INNER_PARALLELISM_NAMESPACE_END

/// @macro Opens the `ecst::inner_parallelism::strategy` namespace.
#define ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE \
    ECST_IMPL_INNER_PARALLELISM_STRATEGY_NAMESPACE

/// @macro Closes the `ecst::inner_parallelism::strategy` namespace.
#define ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END \
    ECST_IMPL_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
