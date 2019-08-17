// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./utils.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/ctx/system.hpp>
#include <ecst/mp.hpp>
#include <ecst/settings.hpp>
#include <ecst/sig_list.hpp>

namespace ecst::ctx::storage::system::impl
{
    template <typename TSettings>
    class data
    {
    private:
        using ssl_type = decltype(settings::system_sig_list(TSettings{}));

        using storage_type = mp::list::unwrap_bh_tuple<
            system_storage_tuple_type<TSettings, ssl_type>>;

        template <typename TSystem>
        using instance_for = // .
            ctx::system::instance<TSettings, TSystem>;

        template <typename TSystem>
        using system_sig_for =
            decltype(sig_list::system::sig_by_type<TSystem>(ssl_type{}));

        template <typename TSystem>
        using system_tag_for = sig::system::tag_type<system_sig_for<TSystem>>;

        template <typename TSystemTag>
        using tag_to_system = tag::system::unwrap<TSystemTag>;

        storage_type _storage;

        /// @brief Returns a reference to the instance storing a `TSystem`
        /// instance.
        template <typename TSystem, typename TSelf>
        static auto& instance_impl(TSelf&& self) noexcept
        {
            // Find the index of the first instance storing `TSystem`.
            // Assumes that at least one instances does so.
            auto idx = mp::list::index_of_first_matching(self._storage,
                [](const auto& x) { return x.template system_is<TSystem>(); });

            // Return a reference to it.
            return bh::at(self._storage, idx);
        }

    public:
        constexpr auto system_count() const noexcept
        {
            return bh::size(ssl_type{});
        }

        template <typename TSystem>
        auto& instance() noexcept
        {
            return instance_impl<TSystem>(*this);
        }

        template <typename TSystem>
        const auto& instance() const noexcept
        {
            return instance_impl<TSystem>(*this);
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
        auto& system_by_sig(TSystemSignature) noexcept
        {
            return instance_by_tag(sig::system::tag_type<TSystemSignature>{});
        }

    private:
        template <typename TSelf, typename TF>
        static void for_all_instances_impl(TSelf&& self, TF&& f)
        {
            bh::for_each(self._storage, f);
        }

        template <typename TSelf, typename TF, typename TKind>
        static void for_instances_of_kind_impl(TSelf&& self, TF&& f, TKind)
        {
            // TODO: probably slow
            bh::for_each(self._storage, [&f](auto& x) {
                if constexpr(x.kind_is(TKind{}))
                {
                    f(x);
                }
            });
        }

    public:
        template <typename TF>
        void for_all_instances(TF&& f)
        {
            for_all_instances_impl(*this, FWD(f));
        }

        template <typename TF, typename TKind>
        void for_instances_of_kind(TF&& f, TKind kind)
        {
            for_instances_of_kind_impl(*this, FWD(f), kind);
        }

        constexpr auto all_instances_count() const noexcept
        {
            return system_count();
        }

        template <typename TKind>
        constexpr auto instances_of_kind_count(TKind kind) const noexcept
        {
            return bh::count_if(
                _storage, [&kind](auto&& x) { return x.kind_is(kind); });
        }
    };
} // namespace ecst::ctx::storage::system::impl

namespace ecst::ctx::storage::system
{
    template <typename TSettings>
    using dispatch = impl::data<TSettings>;
} // namespace ecst::ctx::storage::system
