// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/debug.hpp>
#include <ecst/utils.hpp>
#include <ecst/signature/system.hpp>
#include "../state.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        namespace state_manager
        {
            /// @brief Data type, output of `TSystemSignature`.
            template <typename TSystemSignature>
            using data_type = // .
                typename signature::system::output_type<TSystemSignature>::type;

            /// @brief Storage for a "system state" and its eventual output
            /// data.
            template <typename TSettings, typename TSystemSignature>
            class state_storage : public data_type<TSystemSignature>
            {
            private:
                using my_data_type = data_type<TSystemSignature>;

                state<TSettings> _state;

            public:
                state_storage() = default;
                ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(state_storage);

                void clear() noexcept
                {
                    as_state().clear();
                }

                auto& as_data() noexcept
                {
                    return vrmc::to_base<my_data_type>(*this);
                }

                const auto& as_data() const noexcept
                {
                    return vrmc::to_base<my_data_type>(*this);
                }

                auto& as_state() noexcept
                {
                    return _state;
                }

                const auto& as_state() const noexcept
                {
                    return _state;
                }
            };
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
