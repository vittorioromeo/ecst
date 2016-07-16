// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/signature.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    // TODO: add `subtask_index() const noexcept` getter.

    namespace data_proxy
    {
        /// @brief "Data proxy".
        template <                     // .
            typename TSystemSignature, // .
            typename TEDFunctions,     // .
            typename TContext          // .
            >
        class base
        {
        public:
            using system_signature_type = TSystemSignature;
            using context_type = TContext;
            using settings_type = typename context_type::settings_type;

        private:
            using ed_functions_type = TEDFunctions;

        protected:
            ed_functions_type _functions;

        private:
            context_type& _context;

        private:
            sz_t _ep_count;

            auto& state_wrapper() noexcept;
            auto& state() noexcept;
            auto& output_data() noexcept;

            /// @brief Returns `true` if it's safe to iterate through
            /// `TSystem`'s output values.
            template <typename TSystemTag>
            constexpr auto can_get_output_of(TSystemTag st) noexcept;

        public:
            base(                              // .
                ed_functions_type&& functions, // .
                context_type& context,         // .
                sz_t ep_count                  // .
                ) noexcept;

            /// @brief Iterates over entities assigned to the current subtask.
            /// @details Iterates over all entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_entities(TF&& f);

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept;

            template <typename TComponentTag>
            decltype(auto) get(TComponentTag ct, entity_id eid) noexcept;

            template <typename TF>
            void defer(TF&& f);

            void kill_entity(entity_id eid);

            // TODO: enable if, rename?
            auto& output() noexcept;

            template <typename TSystemTag>
            auto& system(TSystemTag st) noexcept;

            template <typename TSystemTag, typename TF>
            decltype(auto) for_previous_outputs(TSystemTag st, TF&& f) noexcept;
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
