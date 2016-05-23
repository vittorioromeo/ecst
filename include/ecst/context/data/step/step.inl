// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./step.hpp"
#include "../data/data.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace step
        {
            template <typename TSettings>
            proxy<TSettings>::proxy(context_type& context,
                refresh_state_type& refresh_state) noexcept
                : base_type{context, refresh_state}
            {
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::execute_systems_from(TSystemTag st)
            {
                return this->context().execute_systems_from(
                    this->context(), st);
            }
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
