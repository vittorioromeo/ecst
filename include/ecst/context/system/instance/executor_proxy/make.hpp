// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./impl.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace executor_proxy
    {
        // TODO: docs/refactor
        template <typename TInstance, typename... TFs>
        auto make(TInstance& instance, TFs&&... fs) noexcept
        {
            return data<TInstance, decay_t<TFs>...>{instance, FWD(fs)...};
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
