// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/ctx/system.hpp>
#include <ecst/mp.hpp>
#include <ecst/settings.hpp>
#include <ecst/sig_list.hpp>

namespace ecst::ctx::storage::system
{
    template <typename TSettings, typename TSystemSignatureList>
    constexpr auto make_system_storage_tuple(TSystemSignatureList ssl) noexcept
    {
        return bh::transform(ssl, [](auto ssig) {
            // TODO: dispatch depending on system type
            auto uws = mp::unwrapped(ssig);

            using namespace ctx;
            using namespace sig::system;

            // TODO: avoid all this wrapping/unwrapping

            if constexpr(uws.is_kind(sig::system::impl::kind::stateless))
            {
                return mp::type_c< // .
                    ctx::system::stateless_instance<TSettings,
                        decltype(mp::wrap(uws))> // .
                    >;
            }
            else if constexpr(uws.is_kind(
                                  sig::system::impl::kind::stateful))
            {
                return mp::type_c< // .
                    ctx::system::stateful_instance<TSettings,
                        decltype(mp::wrap(uws))> // .
                    >;
            }
            else if constexpr(uws.is_kind(
                                  sig::system::impl::kind::entity))
            {
                return mp::type_c< // .
                    ctx::system::instance<TSettings,
                        decltype(mp::wrap(uws))> // .
                    >;
            }
            else
            {
                struct nope;
                return nope{};
            }
        });
    }

    template <typename TSettings, typename TSystemSignatureList>
    using system_storage_tuple_type =
        decltype(make_system_storage_tuple<TSettings>(TSystemSignatureList{}));
} // namespace ecst::ctx::storage::system
