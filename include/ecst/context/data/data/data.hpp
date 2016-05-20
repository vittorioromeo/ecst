// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include "../system_manager.hpp"
#include "../main_storage.hpp"
#include "../step.hpp"
#include "../defer.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class data : public system_manager<TSettings>,
                     public main_storage<TSettings>
        {
            template <typename>
            friend class defer::proxy;

            template <typename>
            friend class step::proxy;

        public:
            using settings_type = TSettings;
            using handle = ecst::context::entity::impl::handle;

        private:
            using defer_proxy_type = defer::proxy<settings_type>;
            using step_proxy_type = step::proxy<settings_type>;
            using refresh_state_type = defer::refresh_state<settings_type>;

            refresh_state_type _refresh_state;

            /// @brief Executes all systems' deferred functions, sequentially.
            template <typename TRefreshState>
            void refresh_impl_execute_deferred(TRefreshState& rs);

            /// @brief Unsubscribes dead entities from systems, in parallel.
            template <typename TRefreshState>
            void refresh_impl_kill_entities(TRefreshState& rs);

            /// @brief Matches new/modified entities to systems, in parallel.
            template <typename TRefreshState>
            void refresh_impl_match_entities(TRefreshState& rs);

            /// @brief Creates a refresh state and executes refresh steps.
            void refresh_impl();

            /// @brief Refreshes the context.
            void refresh();

        public:
            template <typename TF>
            auto step(TF&& f);
        };
    }
}
ECST_CONTEXT_NAMESPACE_END
