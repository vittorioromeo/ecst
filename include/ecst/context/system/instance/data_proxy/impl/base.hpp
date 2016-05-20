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

            auto& state_wrapper() noexcept
            {
                return _functions._f_state_getter();
            }

            auto& state() noexcept
            {
                return state_wrapper()._state;
            }

            auto& output_data() noexcept
            {
                return state_wrapper().as_data();
            }

            // TODO: move, beautify, refactor
            /// @brief Returns `true` if it's safe to iterate through
            /// `TSystem`'s output values.
            template <typename TSystem>
            constexpr auto can_get_output_of()
            {
                constexpr auto ssl =
                    settings::system_signature_list(settings_type{});

                constexpr auto sig =
                    signature_list::system::signature_by_type<TSystem>(ssl);

                return signature_list::system::has_dependency_recursive(
                    ssl, TSystemSignature{}, sig);
            }

        public:
            base(                                              // .
                ed_functions_type&& functions,                 // .
                context_type& context,                         // .
                sz_t ep_count                                  // .
                ) noexcept : _functions(std::move(functions)), // .
                             _context(context),                // .
                             _ep_count{ep_count}               // .
            {
            }

            /// @brief Iterates over entities assigned to the current subtask.
            /// @details Iterates over all entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_entities(TF&& f)
            {
                return _functions._f_for_entities(f);
            }

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept
            {
                return _ep_count;
            }

            template <typename TComponentTag>
            decltype(auto) get(TComponentTag ct, entity_id eid) noexcept
            {
                // TODO: static assert validity!!!
                return _context.get_component(ct, eid);
                /*
                    using component_type =
                        signature::component::unwrap_tag<TComponentTag>;
                */
            }

            template <typename TF>
            void defer(TF&& f)
            {
                state().add_deferred_fn(FWD(f));
            }

            void kill_entity(entity_id eid)
            {
                state().add_to_kill(eid);
            }

            // TODO: enable if, rename?
            auto& output() noexcept
            {
                ECST_S_ASSERT(signature::system::has_data_output<
                    system_signature_type>());

                return output_data();
            }

            template <typename TSystemTag>
            auto& system(TSystemTag st) noexcept
            {
                return _context.system(st);
            }

            template <typename TSystemTag, typename TF>
            decltype(auto) for_previous_outputs(TSystemTag st, TF&& f) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;
                ECST_S_ASSERT_DT(can_get_output_of<system_type>());
                return _context.for_system_outputs(st, FWD(f));
            }
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
