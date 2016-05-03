// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

ECST_INNER_PARALLELISM_NAMESPACE
{
    template <typename TParameters>
    using executor_type =
        typename TParameters::template executor_type<TParameters>;
}
ECST_INNER_PARALLELISM_NAMESPACE_END
