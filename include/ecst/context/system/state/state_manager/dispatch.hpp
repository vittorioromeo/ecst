// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./multi_fixed.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        namespace state_manager
        {
            /// @brief Dispatches the state manager type. (TODO)
            template <typename TSettings, typename TSystemSignature>
            using dispatch = multi_fixed<TSettings, TSystemSignature>;
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
