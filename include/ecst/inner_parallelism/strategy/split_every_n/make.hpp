// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./parameters.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE
{
    namespace split_every_n
    {
        template <typename TEntitiesPerSubtask>
        constexpr auto v(TEntitiesPerSubtask)
        {
            return impl::parameters<TEntitiesPerSubtask>{};
        }
    } // namespace split_every_n
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
