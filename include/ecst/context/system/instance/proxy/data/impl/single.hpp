// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/utils.hpp>
#include "./base.hpp"

#define ECST_IMPL_DP_SINGLE_BASE                \
    base<TSystemSignature, TContext, TInstance, \
        single<TSystemSignature, TContext, TInstance>>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        /// @brief Single-subtask data proxy.
        /// @details Created during non-parallel instance execution.
        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TInstance         // .
            >
        class single : public ECST_IMPL_DP_SINGLE_BASE
        {
        private:
            using base_type = ECST_IMPL_DP_SINGLE_BASE;
            friend base_type;

            /// @brief Returns a reference to the state associated with this
            /// subtask.
            auto& state_wrapper() noexcept;

        public:
            using system_signature_type =
                typename base_type::system_signature_type;

            using settings_type = typename base_type::settings_type;

            using base_type::base_type;

            /// @brief Iterates over entities assigned to the current subtask.
            /// @details Iterates over all entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_entities(TF&& f);

            /// @brief Iterates over entities not assigned to the current
            /// subtask.
            /// @details Iterates over no entities if the system has a single
            /// subtask.
            template <typename TF>
            constexpr auto for_other_entities(TF&&) noexcept;

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f);

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept;

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept;

            /// @brief Count of entities not in the current subtask.
            constexpr auto other_entity_count() const noexcept;

            /// @brief Returns the index of the current subtask.
            constexpr auto subtask_index() const noexcept;
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_DP_SINGLE_BASE
