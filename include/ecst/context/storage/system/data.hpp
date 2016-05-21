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
#include <ecst/context/system.hpp>
#include "./utils.hpp"

ECST_CONTEXT_STORAGE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class data
        {
        private:
            using ssl_type =
                settings::impl::ctx_system_signature_list<TSettings>;

            using storage_type = mp::list::unwrap_tuple<
                system_storage_tuple_type<TSettings, ssl_type>>;

            template <typename TSystem>
            using instance_for = // .
                context::system::instance<TSettings, TSystem>;

            template <typename TSystem>
            using system_signature_for = decltype(
                signature_list::system::signature_by_type<TSystem>(ssl_type{}));

            template <typename TSystem>
            using system_tag_for = signature::system::impl::signature_tag_type<
                system_signature_for<TSystem>>;

            template <typename TSystemTag>
            using tag_to_system = signature::system::unwrap_tag<TSystemTag>;

            storage_type _storage;

        public:
            auto system_count() const noexcept
            {
                return mp::bh::size(ssl_type{});
            }

            template <typename TSystem>
            auto& instance() noexcept
            {
                return std::get<instance_for<system_signature_for<TSystem>>>(
                    _storage);
            }

            template <typename TSystem>
            const auto& instance() const noexcept
            {
                return std::get<instance_for<system_signature_for<TSystem>>>(
                    _storage);
            }

            template <typename TSystemID>
            auto& system_by_id(TSystemID) noexcept
            {
                return std::get<TSystemID{}>(_storage);
            }

            template <typename TSystemTag>
            auto& system_by_tag(TSystemTag) noexcept
            {
                return instance<tag_to_system<TSystemTag>>();
            }

            template <typename TSystemSignature>
            auto& system_by_signature(TSystemSignature) noexcept
            {
                return system_by_tag(
                    signature::system::impl::signature_tag_type<
                        TSystemSignature>{});
            }

            template <typename TF>
            void for_systems(TF&& f)
            {
                for_tuple(f, _storage);
            }
        };
    }

    template <typename TSettings>
    using dispatch = impl::data<TSettings>;
}
ECST_CONTEXT_STORAGE_SYSTEM_NAMESPACE_END
