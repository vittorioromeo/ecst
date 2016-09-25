// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
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
                decltype(settings::system_signature_list(TSettings{}));

            using storage_type = mp::list::unwrap_bh_tuple<
                system_storage_tuple_type<TSettings, ssl_type>>;

            template <typename TSystem>
            using instance_for = // .
                context::system::instance<TSettings, TSystem>;

            template <typename TSystem>
            using system_signature_for = decltype(
                signature_list::system::signature_by_type<TSystem>(ssl_type{}));

            template <typename TSystem>
            using system_tag_for =
                signature::system::tag_type<system_signature_for<TSystem>>;

            template <typename TSystemTag>
            using tag_to_system = tag::system::unwrap<TSystemTag>;

            storage_type _storage;

        public:
            auto system_count() const noexcept
            {
                return bh::size(ssl_type{});
            }

            template <typename TSystem>
            auto& instance() noexcept
            {
                auto idx =
                    mp::list::index_of_first_matching(_storage, [](auto&& x)
                        {
                            using inner_st =
                                typename ECST_DECAY_DECLTYPE(x)::system_type;

                            return std::is_same<inner_st, TSystem>{};
                        });

                return bh::at(_storage, idx);
            }

            template <typename TSystem>
            const auto& instance() const noexcept
            {
                auto idx =
                    mp::list::index_of_first_matching(_storage, [](auto&& x)
                        {
                            using inner_st =
                                typename ECST_DECAY_DECLTYPE(x)::system_type;

                            return std::is_same<inner_st, TSystem>{};
                        });

                return bh::at(_storage, idx);
            }

            template <typename TSystemID>
            auto& instance_by_id(TSystemID sid) noexcept
            {
                return bh::at(_storage, sid);
            }

            template <typename TSystemTag>
            auto& instance_by_tag(TSystemTag) noexcept
            {
                return instance<tag_to_system<TSystemTag>>();
            }

            template <typename TSystemTag>
            const auto& instance_by_tag(TSystemTag) const noexcept
            {
                return instance<tag_to_system<TSystemTag>>();
            }

            template <typename TSystemSignature>
            auto& system_by_signature(TSystemSignature) noexcept
            {
                return instance_by_tag(
                    signature::system::tag_type<TSystemSignature>{});
            }

            template <typename TF>
            void for_instances(TF&& f)
            {
                bh::for_each(_storage, f);
            }
        };
    }

    template <typename TSettings>
    using dispatch = impl::data<TSettings>;
}
ECST_CONTEXT_STORAGE_SYSTEM_NAMESPACE_END
