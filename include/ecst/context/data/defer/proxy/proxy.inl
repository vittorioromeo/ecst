// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./proxy.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace defer
        {
            template <typename TSettings>
            proxy<TSettings>::proxy(context_type& context,
                refresh_state_type& refresh_state) noexcept
                : _context{context},
                  _refresh_state{refresh_state}
            {
            }

            template <typename TSettings>
            auto& ECST_PURE_FN proxy<TSettings>::context() noexcept
            {
                return _context;
            }

            template <typename TSettings>
            entity_id proxy<TSettings>::create_entity()
            {
                // Create an entity ID and allocate metadata.
                auto res(_context.create_entity());

                // The newly-created it must be matched to systems.
                _refresh_state.add_to_match(res);

                // Return the entity ID.
                return res;
            }

            template <typename TSettings>
            handle proxy<TSettings>::create_handle(entity_id eid) noexcept
            {
                return _context.create_handle(eid);
            }

            template <typename TSettings>
            handle proxy<TSettings>::create_entity_and_handle()
            {
                auto eid(create_entity());
                return create_handle(eid);
            }

            template <typename TSettings>
            auto proxy<TSettings>::valid_handle(const handle& h) const noexcept
            {
                return _context.valid_handle(h);
            }

            template <typename TSettings>
            auto proxy<TSettings>::access(const handle& h) const noexcept
            {
                return _context.access(h);
            }


            template <typename TSettings>
            void proxy<TSettings>::kill_entity(entity_id eid) noexcept
            {
                // The killed entity must be removed from all systems.
                _refresh_state.add_to_kill(eid);
            }

            template <typename TSettings>
            template <typename TComponent>
            decltype(auto) proxy<TSettings>::add_component(entity_id eid)
            {
                // The modified entity must be re-matched.
                _refresh_state.add_to_match(eid);
                return _context.template add_component<TComponent>(eid);
            }

            template <typename TSettings>
            template <typename TComponent>
            decltype(auto) ECST_PURE_FN proxy<TSettings>::get_component(
                entity_id eid)
            {
                return _context.template get_component<TComponent>(eid);
            }

            template <typename TSettings>
            template <typename TComponent>
            void proxy<TSettings>::remove_component(entity_id eid) noexcept
            {
                // Un-set component bit and clear chunk metadata.
                _context.template remove_component<TComponent>(eid);

                // The modified entity must be re-matched.
                _refresh_state.add_to_match(eid);
            }

            template <typename TSettings>
            template <typename TComponentTag>
            decltype(auto) proxy<TSettings>::add_component(
                TComponentTag, entity_id eid)
            {
                return add_component<component_from_tag<TComponentTag>>(eid);
            }

            template <typename TSettings>
            template <typename TComponentTag>
            decltype(auto) ECST_PURE_FN proxy<TSettings>::get_component(
                TComponentTag, entity_id eid)
            {
                return get_component<component_from_tag<TComponentTag>>(eid);
            }

            template <typename TSettings>
            template <typename TComponentTag>
            void proxy<TSettings>::remove_component(
                TComponentTag, entity_id eid) noexcept
            {
                remove_component<component_from_tag<TComponentTag>>(eid);
            }

            template <typename TSettings>
            template <typename TSystem>
            auto& ECST_PURE_FN proxy<TSettings>::instance() noexcept
            {
                return _context.template instance<TSystem>();
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto& ECST_PURE_FN proxy<TSettings>::instance(TSystemTag) noexcept
            {
                return instance<system_from_tag<TSystemTag>>();
            }

            template <typename TSettings>
            template <typename TSystem>
            auto& ECST_PURE_FN proxy<TSettings>::system() noexcept
            {
                return _context.template instance<TSystem>().system();
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto& ECST_PURE_FN proxy<TSettings>::system(TSystemTag) noexcept
            {
                return system<system_from_tag<TSystemTag>>();
            }

            template <typename TSettings>
            template <typename TSystem, typename TF>
            decltype(auto) proxy<TSettings>::for_system_outputs(TF&& f)
            {
                return _context.template for_system_outputs<TSystem>(FWD(f));
            }

            template <typename TSettings>
            template <typename TSystemTag, typename TF>
            decltype(auto) proxy<TSettings>::for_system_outputs(
                TSystemTag, TF&& f)
            {
                return for_system_outputs<system_from_tag<TSystemTag>>(FWD(f));
            }

            template <typename TSettings>
            template <typename TSystem, typename TAcc, typename TF>
            auto proxy<TSettings>::foldl_system_outputs(TAcc acc, TF&& f)
            {
                return _context.template foldl_system_outputs<TSystem>(
                    acc, FWD(f));
            }

            template <typename TSettings>
            template <typename TSystemTag, typename TAcc, typename TF>
            auto proxy<TSettings>::foldl_system_outputs(
                TSystemTag, TAcc acc, TF&& f)
            {
                return foldl_system_outputs<system_from_tag<TSystemTag>>(
                    acc, FWD(f));
            }

            template <typename TSettings>
            template <typename TSystem>
            auto proxy<TSettings>::is_in_system(entity_id eid) const noexcept
            {
                return _context.template is_in_system<TSystem>(eid);
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::is_in_system(TSystemTag, entity_id eid) const
                noexcept
            {
                return _context.is_in_system(TSystemTag{}, eid);
            }

            template <typename TSettings>
            template <typename TSystem>
            auto proxy<TSettings>::count_entities_in() const noexcept
            {
                return _context.template count_entities_in<TSystem>();
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::count_entities_in(TSystemTag) const noexcept
            {
                return _context.count_entities_in(TSystemTag{});
            }

            template <typename TSettings>
            template <typename TSystem>
            auto proxy<TSettings>::any_entity_in() const noexcept
            {
                return _context.template any_entity_in<TSystem>();
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::any_entity_in(TSystemTag) const noexcept
            {
                return _context.any_entity_in(TSystemTag{});
            }
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
