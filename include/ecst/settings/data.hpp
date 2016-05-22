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
    // TODO: cleanup

    namespace impl
    {
        namespace keys
        {
            constexpr auto threading = sz_v<0>;
            constexpr auto entity_storage = sz_v<1>;
            constexpr auto component_signature_list = sz_v<2>;
            constexpr auto system_signature_list = sz_v<3>;
            constexpr auto scheduler = sz_v<4>;
            constexpr auto refresh_parallelism = sz_v<5>;
        }
    }

    namespace impl
    {


        template <typename TOptions>
        class data
        {
        public:
// TODO:
#define TEMP(x) mp::list::option_type<TOptions, decltype(x)>

            using multithreading = TEMP(keys::threading);
            using entity_storage = TEMP(keys::entity_storage);
            using system_signature_list = TEMP(keys::system_signature_list);
            using component_signature_list = TEMP(
                keys::component_signature_list);
            using scheduler_type = TEMP(keys::scheduler);
            using refresh_parallelism = TEMP(keys::refresh_parallelism);

#undef TEMP
            // TODO:
            // ECST_S_ASSERT(threading::is<TMultithreading>);
            // ECST_S_ASSERT(is_entity_storage<TEntityStorage>);

            ECST_S_ASSERT(
                signature_list::component::valid(component_signature_list{}));

            ECST_S_ASSERT(
                signature_list::system::valid(system_signature_list{}));

            // ECST_S_ASSERT(refresh_parallelism::is<TRefreshParallelism>);

            // TODO: is_scheduler



        private:
            template <typename TKey, typename T>
            constexpr auto change_self(const TKey& key, T&& x) noexcept
            {
                auto new_options =
                    mp::list::set_option(TOptions{}, key, FWD(x));
                return data<decltype(new_options)>{};
            }

        public:
            constexpr auto allow_inner_parallelism() noexcept
            {
                return change_self(
                    keys::threading, impl::v_allow_inner_parallelism);
            }

            constexpr auto disallow_inner_parallelism() noexcept
            {
                return change_self(
                    keys::threading, impl::v_disallow_inner_parallelism);
            }

            constexpr auto singlethreaded() noexcept
            {
                return change_self(keys::threading, impl::v_singlethreaded);
            }

            template <typename TCapacity>
            constexpr auto fixed_entity_limit(TCapacity) noexcept
            {
                return change_self(
                    keys::entity_storage, impl::fixed_impl<TCapacity>{});
            }

            template <typename TInitialCapacity>
            constexpr auto dynamic_entity_limit(TInitialCapacity) noexcept
            {
                return change_self(keys::entity_storage,
                    impl::dynamic_impl<TInitialCapacity>{});
            }

            template <typename TNewComponentSignatureList>
            constexpr auto component_signatures(
                TNewComponentSignatureList new_csl) noexcept
            {
                return change_self(keys::component_signature_list, new_csl);
            }

            template <typename TNewSystemSignatureList>
            constexpr auto system_signatures(
                TNewSystemSignatureList new_ssl) noexcept
            {
                return change_self(keys::system_signature_list, new_ssl);
            }

            template <typename TNewScheduler>
            constexpr auto scheduler(TNewScheduler new_scheduler) noexcept
            {
                return change_self(keys::scheduler, new_scheduler);
            }
        };

        template <typename TSettings>
        using ctx_system_signature_list =
            typename TSettings::system_signature_list;

        template <typename TSettings>
        using ctx_component_signature_list =
            typename TSettings::component_signature_list;

        template <typename TSettings>
        using ctx_scheduler = typename TSettings::scheduler_type;
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
        return mp::bh::bool_c<settings::multithreading<TSettings>{} ==
                              impl::v_allow_inner_parallelism>;
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
