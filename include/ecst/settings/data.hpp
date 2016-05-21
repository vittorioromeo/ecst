// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/signature_list.hpp>
#include "./data_settings.hpp"

ECST_SETTINGS_NAMESPACE
{
    namespace impl
    {
        template <                            // .
            typename TMultithreading,         // .
            typename TEntityStorage,          // .
            typename TComponentSignatureList, // .
            typename TSystemSignatureList,    // .
            typename TScheduler,              // .
            typename TRefreshParallelism      // .
            >
        struct data
        {
            ECST_S_ASSERT(threading::is<TMultithreading>);
            ECST_S_ASSERT(is_entity_storage<TEntityStorage>);
            ECST_S_ASSERT(signature_list::component::is_signature_list(
                TComponentSignatureList{}));
            ECST_S_ASSERT(signature_list::system::is_signature_list(
                TSystemSignatureList{}));
            ECST_S_ASSERT(refresh_parallelism::is<TRefreshParallelism>);

            // TODO: is_scheduler

            using multithreading = TMultithreading;
            using entity_storage = TEntityStorage;
            using system_signature_list = TSystemSignatureList;
            using component_signature_list = TComponentSignatureList;
            using scheduler = TScheduler;
            using refresh_parallelism = TRefreshParallelism;
        };

        template <typename TSettings>
        using ctx_system_signature_list =
            typename TSettings::system_signature_list;

        template <typename TSettings>
        using ctx_component_signature_list =
            typename TSettings::component_signature_list;

        template <typename TSettings>
        using ctx_scheduler = typename TSettings::scheduler;
    }

    template <typename TSettings>
    constexpr auto system_signature_list(TSettings)
    {
        return impl::ctx_system_signature_list<TSettings>{};
    }

    template <typename TSettings>
    constexpr auto component_signature_list(TSettings)
    {
        return impl::ctx_component_signature_list<TSettings>{};
    }

    template <typename TSettings>
    using entity_storage_type = typename TSettings::entity_storage;

    template <typename TSettings>
    constexpr auto has_fixed_entity_storage = // .
        is_fixed_entity_storage<entity_storage_type<TSettings>>;

    template <typename TSettings>
    constexpr auto has_dynamic_entity_storage = // .
        is_dynamic_entity_storage<entity_storage_type<TSettings>>;

    namespace impl
    {
        template <typename TSettings>
        constexpr auto fixed_capacity_v = // .
            entity_storage_type<TSettings>::size::value;

        template <typename TSettings>
        constexpr auto dynamic_initial_capacity_v = // .
            entity_storage_type<TSettings>::initial_size::value;
    }

    template <typename TSettings>
    constexpr auto fixed_capacity(TSettings)
    {
        ECST_S_ASSERT(has_fixed_entity_storage<TSettings>);
        return impl::fixed_capacity_v<TSettings>;
    }

    template <typename TSettings>
    constexpr auto initial_capacity(TSettings)
    {
        ECST_S_ASSERT(has_dynamic_entity_storage<TSettings>);
        return impl::dynamic_initial_capacity_v<TSettings>;
    }

    template <typename TSettings>
    using multithreading = // .
        typename TSettings::multithreading;

    template <typename TSettings>
    using refresh_parallelism = // .
        typename TSettings::refresh_parallelism;

    template <typename TSettings>
    constexpr auto system_count()
    {
        return mp::bh::size(system_signature_list(TSettings{}));
    }

    template <typename TSettings>
    constexpr auto inner_parallelism_allowed()
    {
        return settings::impl::allows_inner_parallelism< // .
            settings::multithreading<TSettings>          // .
            >();
    }

    template <typename TSettings>
    constexpr auto refresh_parallelism_allowed()
    {
        return settings::impl::allows_refresh_parallelism< // .
            settings::refresh_parallelism<TSettings>       // .
            >();
    }
}
ECST_SETTINGS_NAMESPACE_END
