// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/utils.hpp>
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
        class single : public base<TSystemSignature, TEDFunctions, TContext>
        {
        private:
            using base_type = base<TSystemSignature, TEDFunctions, TContext>;

        public:
            using system_signature_type =
                typename base_type::system_signature_type;

            using settings_type = typename base_type::settings_type;

            using base_type::base_type;

            /// @brief Iterates over entities not assigned to the current
            /// subtask.
            /// @details Iterates over no entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_other_entities(TF&&) noexcept
            {
                return ecst::impl::do_nothing;
            }

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f)
            {
                return this->for_entities(FWD(f));
            }

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept
            {
                return this->entity_count();
            }

            /// @brief Count of entities not in the current subtask.
            constexpr auto other_entity_count() const noexcept
            {
                return sz_t(0);
            }
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
