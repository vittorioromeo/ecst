// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/utils.hpp>
#include "./base.hpp"

#define ECST_IMPL_DATA_PROXY_SINGLE_BASE        \
    base<TSystemSignature, TContext, TInstance, \
        single<TSystemSignature, TContext, TInstance>>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        // TODO:
        /// @brief "Data proxy".
        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TInstance         // .
            >
        class single : public ECST_IMPL_DATA_PROXY_SINGLE_BASE
        {
        private:
            using base_type = ECST_IMPL_DATA_PROXY_SINGLE_BASE;

        public:
            using system_signature_type =
                typename base_type::system_signature_type;

            using settings_type = typename base_type::settings_type;

            using base_type::base_type;

            auto& state_wrapper() noexcept;


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
            auto for_other_entities(TF&&) noexcept;

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f);

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept;

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept;

            /// @brief Count of entities not in the current subtask.
            constexpr auto other_entity_count() const noexcept;
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_DATA_PROXY_SINGLE_BASE
