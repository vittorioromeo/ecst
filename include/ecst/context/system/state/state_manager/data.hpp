// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./state_storage.hpp"
#include <array>
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/context/types.hpp>
#include <ecst/settings.hpp>
#include <ecst/signature/system.hpp>
#include <ecst/utils.hpp>

namespace ecst::context::system::impl::state_manager
{
    /// @brief State manager with a fixed storage.
    /// @details "State storages" are stored in an `std::array`.
    template <                    // .
        typename TInstance,       // .
        typename TSettings,       // .
        typename TSystemSignature // .
        >
    class data
    {
    public:
        using settings_type = TSettings;
        using system_signature_type = TSystemSignature;
        using state_storage_type = // .
            state_storage<settings_type, system_signature_type>;

        static constexpr auto max_subtask_count() noexcept
        {
            return 16;
        }

    private:
        std::array<state_storage_type, max_subtask_count()> _ss_arr;

        // Number of currently prepared (in use) states.
        sz_t _prepared = 0;

        TInstance& _instance;

    public:
        data(TInstance& instance) noexcept : _instance{instance}
        {
        }

        ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(data);

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
} // namespace ecst::context::system::impl::state_manager
