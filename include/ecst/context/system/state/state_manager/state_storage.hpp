// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/debug.hpp>
#include <ecst/settings.hpp>
#include <ecst/signature/system.hpp>
#include "../state.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        namespace state_manager
        {
            template <typename TSystemSignature>
            using data_type = // .
                signature::system::output_data_type<TSystemSignature>;

            template <typename TSettings, typename TSystemSignature>
            class state_storage : public data_type<TSystemSignature>
            {
            public:
                using my_data_type = data_type<TSystemSignature>;

                auto& as_data() noexcept
                {
                    return vrmc::to_base<my_data_type>(*this);
                }

                const auto& as_data() const noexcept
                {
                    return vrmc::to_base<my_data_type>(*this);
                }

                state<TSettings> _state;

                void clear()
                {
                    _state.clear();
                }
            };
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
