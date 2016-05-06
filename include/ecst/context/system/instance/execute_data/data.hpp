// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vector>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/utils/sparse_int_set.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        /// @brief "Data proxy".
        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TEDFunctions      // .
            >
        class single_execute_data
        {
        private:
            using system_signature_type = TSystemSignature;
            using settings_type = typename TContext::settings_type;

        public:
            TContext& _context;
            TEDFunctions _functions;
            sz_t _ep_count;

        public:
            single_execute_data(          // .
                TContext& context,        // .
                TEDFunctions&& functions, // .
                sz_t ep_count             // .
                )
                : _context(context),                // .
                  _functions(std::move(functions)), // .
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

            /// @brief Iterates over entities not assigned to the current
            /// subtask.
            /// @details Iterates over no entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_other_entities(TF&&)
            {
                return [](auto&&...)
                {
                };
            }

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f)
            {
                return for_entities(FWD(f));
            }

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept
            {
                return _ep_count;
            }

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept
            {
                return _ep_count;
            }

            /// @brief Count of entities not in the current subtask.
            auto other_entity_count() const noexcept
            {
                return 0;
            }

            template <typename TComponent>
            decltype(auto) get(entity_id eid) noexcept
            {
                // TODO: static assert validity!!!
                return _context.template get_component<TComponent>(eid);
            }

            template <typename TComponentTag>
            decltype(auto) get(TComponentTag, entity_id eid) noexcept
            {
                using component_type =
                    signature::component::unwrap_tag<TComponentTag>;

                return get<component_type>(eid);
            }

        private:
            auto& state() noexcept
            {
                return _functions._f_state_getter()._state;
            }

            auto& output_data() noexcept
            {
                return _functions._f_state_getter().as_data();
            }

        public:
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

        private:
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

                return bool_v<( // .
                    signature_list::system::has_dependency_recursive(
                        ssl, TSystemSignature{}, sig))>;
            }

        public:
            template <typename TSystem>
            auto& system() noexcept
            {
                ECST_S_ASSERT_DT(can_get_output_of<TSystem>());
                return _context.template system<TSystem>();
            }

            template <typename TSystemTag>
            auto& system(TSystemTag) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;
                return system<system_type>();
            }

            template <typename TSystem, typename TF>
            decltype(auto) for_previous_outputs(TF&& f) noexcept
            {
                ECST_S_ASSERT_DT(can_get_output_of<TSystem>());
                return _context.template for_system_outputs<TSystem>(FWD(f));
            }

            template <typename TSystemTag, typename TF>
            decltype(auto) for_previous_outputs(TSystemTag, TF&& f) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;
                return for_previous_outputs<system_type>(FWD(f));
            }
        };

        // TODO:
        /// @brief "Data proxy".
        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TEDFunctions      // .
            >
        class execute_data
        {
        private:
            using system_signature_type = TSystemSignature;
            using settings_type = typename TContext::settings_type;

        public:
            TContext& _context;
            TEDFunctions _functions;
            sz_t _ep_count, _ae_count, _oe_count;

        public:
            execute_data(                 // .
                TContext& context,        // .
                TEDFunctions&& functions, // .
                sz_t ep_count,            // .
                sz_t ae_count,            // .
                sz_t oe_count             // .
                )
                : _context(context),                // .
                  _functions(std::move(functions)), // .
                  _ep_count{ep_count},              // .
                  _ae_count{ae_count},              // .
                  _oe_count{oe_count}               // .
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

            /// @brief Iterates over entities not assigned to the current
            /// subtask.
            /// @details Iterates over no entities if the system has a single
            /// subtask.
            template <typename TF>
            auto for_other_entities(TF&& f)
            {
                return _functions._f_for_other_entities(f);
            }

            /// @brief Iterates over all entities in the system.
            template <typename TF>
            auto for_all_entities(TF&& f)
            {
                return _functions._f_for_all_entities(f);
            }

            /// @brief Count of entities of the current subtask.
            auto entity_count() const noexcept
            {
                return _ep_count;
            }

            /// @brief Count of all entities in the system.
            auto all_entity_count() const noexcept
            {
                return _ae_count;
            }

            /// @brief Count of entities not in the current subtask.
            auto other_entity_count() const noexcept
            {
                return _oe_count;
            }

            template <typename TComponent>
            decltype(auto) get(entity_id eid) noexcept
            {
                // TODO: static assert validity!!!
                return _context.template get_component<TComponent>(eid);
            }

            template <typename TComponentTag>
            decltype(auto) get(TComponentTag, entity_id eid) noexcept
            {
                using component_type =
                    signature::component::unwrap_tag<TComponentTag>;

                return get<component_type>(eid);
            }

        private:
            auto& state() noexcept
            {
                return _functions._f_state_getter()._state;
            }

            auto& output_data() noexcept
            {
                return _functions._f_state_getter().as_data();
            }

        public:
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

        private:
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

                return bool_v<( // .
                    signature_list::system::has_dependency_recursive(
                        ssl, TSystemSignature{}, sig))>;
            }

        public:
            template <typename TSystem>
            auto& system() noexcept
            {
                ECST_S_ASSERT_DT(can_get_output_of<TSystem>());
                return _context.template system<TSystem>();
            }

            template <typename TSystemTag>
            auto& system(TSystemTag) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;
                return system<system_type>();
            }

            template <typename TSystem, typename TF>
            decltype(auto) for_previous_outputs(TF&& f) noexcept
            {
                ECST_S_ASSERT_DT(can_get_output_of<TSystem>());
                return _context.template for_system_outputs<TSystem>(FWD(f));
            }

            template <typename TSystemTag, typename TF>
            decltype(auto) for_previous_outputs(TSystemTag, TF&& f) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;
                return for_previous_outputs<system_type>(FWD(f));
            }
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
