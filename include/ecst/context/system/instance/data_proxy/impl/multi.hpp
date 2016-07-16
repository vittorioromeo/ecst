// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./base.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        // TODO:
        /// @brief "Data proxy".
        template <                     // .
            typename TSystemSignature, // .
            typename TEDFunctions,     // .
            typename TContext          // .
            >
        class multi : public base<TSystemSignature, TEDFunctions, TContext>
        {
        private:
            using base_type = base<TSystemSignature, TEDFunctions, TContext>;

        public:
            using system_signature_type =
                typename base_type::system_signature_type;

            using settings_type = typename base_type::settings_type;

        private:
            sz_t _ae_count, _oe_count;

        public:
            multi(                                                          // .
                TEDFunctions&& functions, TContext& context, sz_t ep_count, // .
                sz_t ae_count,                                              // .
                sz_t oe_count                                               // .
                ) noexcept;

            /// @brief Iterates over entities not assigned to the current
            /// subtask.
            /// @details Iterates over no entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_other_entities(TF&& f);

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f);

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept;

            /// @brief Count of entities not in the current subtask.
            auto other_entity_count() const noexcept;
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
