// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/context/storage.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class main_storage
        {
        private:
            using settings_type = TSettings;

            using handle = ecst::context::entity::impl::handle;

            using component_storage_type = // .
                context::storage::component::dispatch<settings_type>;

            using entity_storage_type = // .
                context::storage::entity::dispatch<settings_type>;

            template <typename T>
            using component_from_tag = signature::component::unwrap_tag<T>;

            component_storage_type _component_storage;
            entity_storage_type _entity_storage;

        public:
            auto& metadata(entity_id) noexcept;
            auto valid_handle(const handle& h) const noexcept;

            entity_id create_entity();
            handle create_handle(entity_id) noexcept;
            handle create_entity_and_handle();

            auto access(const handle&) const noexcept;

            void reclaim(entity_id) noexcept;

            template <typename TComponent>
            decltype(auto) add_component(entity_id);

            template <typename TComponent>
            decltype(auto) get_component(entity_id) noexcept;

            template <typename TComponent>
            void remove_component(entity_id) noexcept;

            template <typename TComponentTag>
            decltype(auto) add_component(TComponentTag, entity_id);

            template <typename TComponentTag>
            decltype(auto) get_component(TComponentTag, entity_id) noexcept;

            template <typename TComponentTag>
            void remove_component(TComponentTag, entity_id) noexcept;

            auto& entity_storage() noexcept;
            auto& component_storage() noexcept;

            const auto& entity_storage() const noexcept;
            const auto& component_storage() const noexcept;

            auto alive(entity_id) const noexcept;
            auto alive(const handle& h) const noexcept;
        };
    }
}
ECST_CONTEXT_NAMESPACE_END
