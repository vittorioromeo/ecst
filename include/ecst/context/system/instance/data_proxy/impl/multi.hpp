// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./base.hpp"

#define ECST_IMPL_DP_MULTI_BASE                 \
    base<TSystemSignature, TContext, TInstance, \
        multi<TSystemSignature, TContext, TInstance>>

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
        class multi : public ECST_IMPL_DP_MULTI_BASE
        {
        private:
            using base_type = ECST_IMPL_DP_MULTI_BASE;

        public:
            using system_signature_type =
                typename base_type::system_signature_type;

            using settings_type = typename base_type::settings_type;

        private:
            sz_t _state_idx, _i_begin, _i_end;

        public:
            multi(TInstance& instance, TContext& context, sz_t state_idx,
                sz_t i_begin, sz_t i_end) noexcept;

            auto& state_wrapper() noexcept;


            // TODO: docss
            template <typename TF>
            auto for_entities(TF&& f);

            /// @brief Iterates over entities not assigned to the current
            /// subtask.
            /// @details Iterates over no entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_other_entities(TF&& f);

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f);

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept;

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept;

            /// @brief Count of entities not in the current subtask.
            auto other_entity_count() const noexcept;
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_DP_MULTI_BASE
