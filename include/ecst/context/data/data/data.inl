// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/utils.hpp>
#include "../defer.hpp"
#include "./data.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        using handle = ecst::context::entity::impl::handle;

        template <typename TSettings>
        template <typename TF>
        auto data<TSettings>::step(TF&& f)
        {
            // Ensure `refresh()` is called after executing `f`.
            ECST_SCOPE_GUARD([this]
                {
                    this->refresh();
                });

            // Clear refresh state.
            _refresh_state.clear();

            // Build context step proxy.
            step_proxy_type step_proxy{*this, _refresh_state};

            // Execute user-defined step.
            return f(step_proxy);
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
