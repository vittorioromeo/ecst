// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/bitset.hpp>
#include <ecst/context/scheduler.hpp>
#include <ecst/context/system.hpp>
#include <ecst/context/storage.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class data;

        namespace defer
        {
            using handle = ecst::context::entity::impl::handle;

            template <typename TSettings>
            class refresh_state;

            template <typename TSettings>
            class proxy
            {
            private:
                using settings_type = TSettings;
                using context_type = data<settings_type>;
                using refresh_state_type = refresh_state<settings_type>;

                context_type& _context;
                refresh_state_type& _refresh_state;

                template <typename T>
                using component_from_tag = signature::component::unwrap_tag<T>;

                template <typename T>
                using system_from_tag = signature::system::unwrap_tag<T>;

            protected:
                auto& context() noexcept;

            public:
                proxy(context_type&, refresh_state_type&) noexcept;

                proxy(const proxy&) = delete;
                proxy& operator=(const proxy&) = delete;

                proxy(proxy&&) = delete;
                proxy& operator=(proxy&&) = delete;

                entity_id create_entity();
                handle create_handle(entity_id) noexcept;
                handle create_entity_and_handle();

                auto valid_handle(const handle& h) const noexcept;
                auto access(const handle&) const noexcept;

                void kill_entity(entity_id) noexcept;

                template <typename TComponent>
                decltype(auto) add_component(entity_id);

                template <typename TComponent>
                decltype(auto) get_component(entity_id);

                template <typename TComponent>
                void remove_component(entity_id) noexcept;

                template <typename TComponentTag>
                decltype(auto) add_component(TComponentTag, entity_id);

                template <typename TComponentTag>
                decltype(auto) get_component(TComponentTag, entity_id);

                template <typename TComponentTag>
                void remove_component(TComponentTag, entity_id) noexcept;

                template <typename TSystem>
                auto& instance() noexcept;

                template <typename TSystemTag>
                auto& instance(TSystemTag) noexcept;

                template <typename TSystem>
                auto& system() noexcept;

                template <typename TSystemTag>
                auto& system(TSystemTag) noexcept;

                template <typename TSystem, typename TF>
                decltype(auto) for_system_outputs(TF&& f);

                template <typename TSystemTag, typename TF>
                decltype(auto) for_system_outputs(TSystemTag, TF&& f);

                template <typename TSystem, typename TAcc, typename TF>
                auto foldl_system_outputs(TAcc acc, TF&& f);

                template <typename TSystemTag, typename TAcc, typename TF>
                auto foldl_system_outputs(TSystemTag, TAcc acc, TF&& f);

                template <typename TSystem>
                auto is_in_system(entity_id) const noexcept;

                template <typename TSystemTag>
                auto is_in_system(TSystemTag, entity_id) const noexcept;

                template <typename TSystem>
                auto count_entities_in() const noexcept;

                template <typename TSystemTag>
                auto count_entities_in(TSystemTag) const noexcept;

                template <typename TSystem>
                auto any_entity_in() const noexcept;

                template <typename TSystemTag>
                auto any_entity_in(TSystemTag) const noexcept;
            };
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
