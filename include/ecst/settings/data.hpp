// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/signature_list.hpp>
#include "./data_settings.hpp"
#include "./impl/keys.hpp"

ECST_SETTINGS_NAMESPACE
{
    // TODO: cleanup

    namespace impl
    {
        template <typename TOptions>
        class data
        {
        private:
            TOptions _map;

        public:
// TODO:
#define TEMP(x) mp::option_map::type_of<TOptions, decltype(x)>

            using entity_storage = TEMP(keys::entity_storage);
            using scheduler_type = TEMP(keys::scheduler);
            using refresh_parallelism = TEMP(keys::refresh_parallelism);

#undef TEMP
            // TODO:
            // ECST_S_ASSERT(threading::is<TMultithreading>);
            // ECST_S_ASSERT(is_entity_storage<TEntityStorage>);


            // ECST_S_ASSERT(refresh_parallelism::is<TRefreshParallelism>);

            // TODO: is_scheduler



        private:
            template <typename TKey, typename T>
            constexpr auto change_self(const TKey& key, T&& x) noexcept
            {
                auto new_options = _map.set(key, FWD(x));
                return data<std::decay_t<decltype(new_options)>>{};
            }

        public:
            template <typename T>
            constexpr auto set_threading(T&& x)
            {
                return change_self(keys::threading, FWD(x));
            }

            template <typename T>
            constexpr auto set_storage(T&& x)
            {
                return change_self(keys::entity_storage, FWD(x));
            }


            // TODO: private
            constexpr auto get_threading() noexcept
            {
                return _map.at(keys::threading);
            }

            constexpr auto get_csl() noexcept
            {
                auto result = _map.at(keys::component_signature_list);
                ECST_S_ASSERT_DT(signature_list::component::valid(result));
                return decltype(result){};
            }

            constexpr auto get_ssl() noexcept
            {
                auto result = _map.at(keys::system_signature_list);
                ECST_S_ASSERT_DT(signature_list::system::valid(result));
                return decltype(result){};
            }



            constexpr auto allow_inner_parallelism() noexcept
            {
                return set_threading(impl::v_allow_inner_parallelism);
            }

            constexpr auto disallow_inner_parallelism() noexcept
            {
                return set_threading(impl::v_disallow_inner_parallelism);
            }

            constexpr auto singlethreaded() noexcept
            {
                return change_self(keys::threading, impl::v_singlethreaded);
            }

            template <typename TCapacity>
            constexpr auto fixed_entity_limit(TCapacity) noexcept
            {
                return set_storage(impl::fixed_impl<TCapacity>{});
            }

            template <typename TInitialCapacity>
            constexpr auto dynamic_entity_limit(TInitialCapacity) noexcept
            {
                return set_storage(impl::dynamic_impl<TInitialCapacity>{});
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
        using ctx_scheduler = typename TSettings::scheduler_type;
    }

    template <typename TSettings>
    constexpr auto system_signature_list(TSettings s)
    {
        return s.get_ssl();
    }

    template <typename TSettings>
    constexpr auto component_signature_list(TSettings s)
    {
        return s.get_csl();
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
    using refresh_parallelism = // .
        typename TSettings::refresh_parallelism;

    template <typename TSettings>
    constexpr auto system_count()
    {
        return bh::size(system_signature_list(TSettings{}));
    }

    template <typename TSettings>
    constexpr auto inner_parallelism_allowed(TSettings s)
    {
        return bh::equal(
            s.get_threading(), impl::v_allow_inner_parallelism);
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
