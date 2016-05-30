// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./main_storage.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        using handle = ecst::context::entity::impl::handle;

        template <typename TSettings>
        auto& ECST_PURE_FN main_storage<TSettings>::metadata(
            entity_id eid) noexcept
        {
            return _entity_storage.get_metadata(eid);
        }

        template <typename TSettings>
        auto main_storage<TSettings>::valid_handle(const handle& h) const
            noexcept
        {
            return _entity_storage.valid_handle(h);
        }

        template <typename TSettings>
        entity_id main_storage<TSettings>::create_entity()
        {
            return _entity_storage.create_entity();
        }

        template <typename TSettings>
        handle main_storage<TSettings>::create_handle(entity_id eid) noexcept
        {
            return _entity_storage.create_handle(eid);
        }

        template <typename TSettings>
        handle main_storage<TSettings>::create_entity_and_handle()
        {
            return _entity_storage.create_entity_and_handle();
        }

        template <typename TSettings>
        auto main_storage<TSettings>::access(const handle& h) const noexcept
        {
            return _entity_storage.access(h);
        }

        template <typename TSettings>
        void main_storage<TSettings>::reclaim(entity_id eid) noexcept
        {
            _entity_storage.reclaim(eid);
        }

        template <typename TSettings>
        template <typename TComponent>
        decltype(auto) main_storage<TSettings>::add_component(entity_id eid)
        {
            return metadata(eid).template add_component<TComponent>(
                _component_storage, eid);
        }

        template <typename TSettings>
        template <typename TComponent>
        decltype(auto) ECST_PURE_FN main_storage<TSettings>::get_component(
            entity_id eid) noexcept
        {
            return metadata(eid).template get_component<TComponent>(
                _component_storage, eid);
        }

        template <typename TSettings>
        template <typename TComponent>
        void main_storage<TSettings>::remove_component(entity_id eid) noexcept
        {
            metadata(eid).template remove_component<TComponent>();
        }

        template <typename TSettings>
        template <typename TComponentTag>
        decltype(auto) main_storage<TSettings>::add_component(
            TComponentTag, entity_id eid)
        {
            return add_component<component_from_tag<TComponentTag>>(eid);
        }

        template <typename TSettings>
        template <typename TComponentTag>
        decltype(auto) ECST_PURE_FN main_storage<TSettings>::get_component(
            TComponentTag, entity_id eid) noexcept
        {
            return get_component<component_from_tag<TComponentTag>>(eid);
        }

        template <typename TSettings>
        template <typename TComponentTag>
        void main_storage<TSettings>::remove_component(
            TComponentTag, entity_id eid) noexcept
        {
            remove_component<component_from_tag<TComponentTag>>(eid);
        }

        template <typename TSettings>
        auto& main_storage<TSettings>::entity_storage() noexcept
        {
            return _entity_storage;
        }

        template <typename TSettings>
        const auto& main_storage<TSettings>::entity_storage() const noexcept
        {
            return _entity_storage;
        }

        template <typename TSettings>
        auto& main_storage<TSettings>::component_storage() noexcept
        {
            return _component_storage;
        }

        template <typename TSettings>
        const auto& main_storage<TSettings>::component_storage() const noexcept
        {
            return _component_storage;
        }

        template <typename TSettings>
        auto main_storage<TSettings>::alive(entity_id eid) const noexcept
        {
            return _entity_storage.alive(eid);
        }

        template <typename TSettings>
        auto main_storage<TSettings>::alive(const handle& h) const noexcept
        {
            return _entity_storage.alive(h);
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
