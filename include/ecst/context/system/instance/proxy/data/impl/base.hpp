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
    namespace data_proxy
    {
        /// @brief "Data proxy".
        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TInstance,        // .
            typename TDerived          // .
            >
        class base
        {
        public:
            using system_signature_type = TSystemSignature;
            using context_type = TContext;
            using instance_type = TInstance;
            using settings_type = typename context_type::settings_type;

        protected:
            instance_type& _instance;
            context_type& _context;

        private:
            auto& subtask_state_wrapper() noexcept;
            auto& subtask_state() noexcept;
            auto& subtask_state_data() noexcept;

            /// @brief Returns `true` if it's safe to iterate through
            /// `TSystem`'s output values.
            template <typename TSystemTag>
            constexpr auto can_get_output_of(TSystemTag st) noexcept;

        public:
            base(instance_type& instance, context_type& context) noexcept;

            // Available for:
            // * Stateless instances;
            // * Stateful instances;
            // * Entity instances.
            template <typename TComponentTag>
            decltype(auto) get(TComponentTag ct, entity_id eid) noexcept;

            // Available for:
            // * Stateful instances;
            // * Entity instances.
            template<typename TComponentTag>
            auto has(TComponentTag ct, entity_id eid) const noexcept;

            template <typename TF>
            void defer(TF&& f);

            // Available for:
            // * Entity instances.
            void kill_entity(entity_id eid);

            // TODO: enable if, rename?
            // Available for:
            // * Stateful instances;
            // * Entity instances.
            auto& output() noexcept;

            // Available for:
            // * Stateless instances;
            // * Stateful instances;
            // * Entity instances.
            template <typename TSystemTag>
            auto& system(TSystemTag st) noexcept;

            // Available for:
            // * Stateless instances;
            // * Stateful instances;
            // * Entity instances.
            template <typename TSystemTag, typename TF>
            decltype(auto) for_previous_outputs(TSystemTag st, TF&& f) noexcept;
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
