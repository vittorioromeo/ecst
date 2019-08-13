// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data_settings.hpp"
#include "./impl/keys.hpp"
#include <ecst/config.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature_list/component/is_signature_list.hpp>
#include <ecst/signature_list/system/is_signature_list.hpp>

namespace ecst::settings
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

            using scheduler_type = TEMP(keys::scheduler);
            using refresh_parallelism = TEMP(keys::refresh_parallelism);

#undef TEMP
            // TODO:
            // static_assertthreading::is<TMultithreading>);
            // static_assertis_entity_storage<TEntityStorage>);


            // static_assert(refresh_parallelism::is<TRefreshParallelism>);

            // TODO: is_scheduler



        private:
            template <typename TKey, typename T>
            constexpr auto change_self(const TKey& key, T&& x) const noexcept
            {
                auto new_options = _map.set(key, FWD(x));
                return data<ECST_DECAY_DECLTYPE(new_options)>{};
            }

        public:
            template <typename T>
            constexpr auto set_threading(T&& x) const noexcept
            {
                return change_self(keys::threading, FWD(x));
            }

            template <typename T>
            constexpr auto set_storage(T&& x) const noexcept
            {
                return change_self(keys::entity_storage, FWD(x));
            }


            // TODO: private (attorney/client pattern?)
            constexpr auto get_threading() const noexcept
            {
                return _map.at(keys::threading);
            }

            constexpr auto get_csl() const noexcept
            {
                auto result = _map.at(keys::component_signature_list);
                ECST_S_ASSERT_DT(signature_list::component::valid(result));
                return decltype(result){};
            }

            constexpr auto get_ssl() const noexcept
            {
                auto result = _map.at(keys::system_signature_list);
                ECST_S_ASSERT_DT(signature_list::system::valid(result));
                return decltype(result){};
            }

            constexpr auto has_fixed_capacity() const noexcept
            {
                auto es = _map.at(keys::entity_storage);
                return is_specialization_of_v<ECST_DECAY_DECLTYPE(es),
                    impl::fixed_impl>;
            }

            constexpr auto has_dynamic_capacity() const noexcept
            {
                auto es = _map.at(keys::entity_storage);
                return is_specialization_of_v<ECST_DECAY_DECLTYPE(es),
                    impl::dynamic_impl>;
            }

            constexpr auto get_fixed_capacity() const noexcept
            {
                ECST_S_ASSERT_DT(has_fixed_capacity());
                auto es = _map.at(keys::entity_storage);
                return typename ECST_DECAY_DECLTYPE(es)::size{};
            }

            constexpr auto get_dynamic_capacity() const noexcept
            {
                ECST_S_ASSERT_DT(has_dynamic_capacity());
                auto es = _map.at(keys::entity_storage);
                return typename ECST_DECAY_DECLTYPE(es)::initial_size{};
            }



            constexpr auto allow_inner_parallelism() const noexcept
            {
                return set_threading(impl::v_allow_inner_parallelism);
            }

            constexpr auto disallow_inner_parallelism() const noexcept
            {
                return set_threading(impl::v_disallow_inner_parallelism);
            }

            constexpr auto singlethreaded() const noexcept
            {
                return change_self(keys::threading, impl::v_singlethreaded);
            }

            template <typename TCapacity>
            constexpr auto fixed_entity_limit(TCapacity) const noexcept
            {
                return set_storage(impl::fixed_impl<TCapacity>{});
            }

            template <typename TInitialCapacity>
            constexpr auto dynamic_entity_limit(TInitialCapacity) const noexcept
            {
                return set_storage(impl::dynamic_impl<TInitialCapacity>{});
            }

            template <typename TNewComponentSignatureList>
            constexpr auto component_signatures(
                TNewComponentSignatureList new_csl) const noexcept
            {
                return change_self(keys::component_signature_list, new_csl);
            }

            template <typename TNewSystemSignatureList>
            constexpr auto system_signatures(
                TNewSystemSignatureList new_ssl) const noexcept
            {
                return change_self(keys::system_signature_list, new_ssl);
            }

            template <typename TNewScheduler>
            constexpr auto scheduler(TNewScheduler new_scheduler) const noexcept
            {
                return change_self(keys::scheduler, new_scheduler);
            }
        };

        template <typename TSettings>
        using ctx_scheduler = typename TSettings::scheduler_type;
    } // namespace impl

    template <typename TSettings, typename TFFixed, typename TFDynamic>
    auto dispatch_on_storage_type(
        TSettings&& s, TFFixed&& f_fixed, TFDynamic&& f_dynamic)
    {
        constexpr bool b = std::decay_t<TSettings>{}.has_fixed_capacity();
        if constexpr(b)
        {
            auto capacity = s.get_fixed_capacity();
            return f_fixed(capacity);
        }
        else
        {
            auto initial_capacity = s.get_dynamic_capacity();
            return f_dynamic(initial_capacity);
        }
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
        return bh::equal(s.get_threading(), impl::v_allow_inner_parallelism);
    }

    template <typename TSettings>
    constexpr auto refresh_parallelism_allowed()
    {
        return settings::impl::allows_refresh_parallelism< // .
            settings::refresh_parallelism<TSettings>       // .
            >();
    }
} // namespace ecst::settings
