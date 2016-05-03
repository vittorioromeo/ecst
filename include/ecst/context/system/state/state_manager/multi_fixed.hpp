// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <array>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/settings.hpp>
#include <ecst/utils.hpp>
#include <ecst/signature/system.hpp>
#include <ecst/context/types.hpp>
#include "./state_storage.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        namespace state_manager
        {
            template <typename TSettings, typename TSystemSignature>
            class multi_fixed
            {
            private:
                static constexpr auto max_subtasks = 16;

            public:
                using settings_type = TSettings;
                using system_signature_type = TSystemSignature;
                using state_storage_type = // .
                    state_storage<settings_type, system_signature_type>;

                static constexpr auto max_subtask_count() noexcept
                {
                    return max_subtasks;
                }

            private:
                std::array<state_storage_type, max_subtask_count()> _ss_arr;
                sz_t _prepared = 0;

            public:
                multi_fixed() = default;
                ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(multi_fixed);

                void clear_and_prepare(sz_t n) noexcept
                {
                    ECST_ASSERT_OP(n, <=, max_subtask_count());
                    _prepared = n;

                    for(sz_t i = 0; i < _prepared; ++i)
                    {
                        _ss_arr[i].clear();
                    }
                }

                template <typename TF>
                void for_states(TF&& f) noexcept
                {
                    for(sz_t i = 0; i < _prepared; ++i)
                    {
                        f(_ss_arr[i]);
                    }
                }

                auto& get(sz_t i) noexcept
                {
                    ECST_ASSERT_OP(i, <, _ss_arr.size());
                    ECST_ASSERT_OP(i, <, _prepared);
                    return _ss_arr[i];
                }
            };
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
