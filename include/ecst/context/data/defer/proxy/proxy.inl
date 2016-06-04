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
                _refresh_state._to_match.add(res);

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
                _refresh_state._to_kill.add(eid);
            }

            template <typename TSettings>
            template <typename TComponentTag>
            decltype(auto) proxy<TSettings>::add_component(
                TComponentTag ct, entity_id eid)
            {
                // The modified entity must be re-matched.
                _refresh_state._to_match.add(eid);
                return _context.add_component(ct, eid);
            }

            template <typename TSettings>
            template <typename TComponentTag>
            decltype(auto) ECST_PURE_FN proxy<TSettings>::get_component(
                TComponentTag ct, entity_id eid)
            {
                return _context.get_component(ct, eid);
            }

            template <typename TSettings>
            template <typename TComponentTag>
            void proxy<TSettings>::remove_component(
                TComponentTag ct, entity_id eid) noexcept
            {
                // Un-set component bit and clear chunk metadata.
                _context.remove_component(ct, eid);

                // The modified entity must be re-matched.
                _refresh_state._to_match.add(eid);
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto& ECST_PURE_FN proxy<TSettings>::instance(
                TSystemTag st) noexcept
            {
                return _context.instance(st);
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto& ECST_PURE_FN proxy<TSettings>::system(TSystemTag st) noexcept
            {
                return _context.system(st);
            }

            template <typename TSettings>
            template <typename TSystemTag, typename TF>
            decltype(auto) proxy<TSettings>::for_system_outputs(
                TSystemTag st, TF&& f)
            {
                return _context.for_system_outputs(st, FWD(f));
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::is_in_system(
                TSystemTag st, entity_id eid) const noexcept
            {
                return _context.is_in_system(st, eid);
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::count_entities_in(TSystemTag st) const
                noexcept
            {
                return _context.count_entities_in(st);
            }

            template <typename TSettings>
            template <typename TSystemTag>
            auto proxy<TSettings>::any_entity_in(TSystemTag st) const noexcept
            {
                return _context.any_entity_in(st);
            }
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
