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

ECST_CONTEXT_STORAGE_SYSTEM_NAMESPACE
{
    template <typename TSettings, typename TSystemSignatureList>
    constexpr auto make_system_storage_tuple(TSystemSignatureList ssl) noexcept
    {
        return bh::transform(ssl, [](auto ssig)
            {
                // TODO: dispatch depending on system type
                auto uws = mp::unwrapped(ssig);

                using namespace context;
                using namespace signature::system;

                // TODO: avoid all this wrapping/unwrapping

                return static_if(uws.is_kind(
                                     signature::system::impl::kind::stateless))
                    .then([](auto s)
                        {
                            return mp::type_c< // .
                                context::system::stateless_instance<TSettings,
                                    decltype(mp::wrap(s))> // .
                                >;
                        })
                    .else_if(
                        uws.is_kind(signature::system::impl::kind::stateful))
                    .then([](auto s)
                        {
                            return mp::type_c< // .
                                context::system::stateful_instance<TSettings,
                                    decltype(mp::wrap(s))> // .
                                >;
                        })
                    .else_if(uws.is_kind(signature::system::impl::kind::entity))
                    .then([](auto s)
                        {
                            return mp::type_c< // .
                                context::system::instance<TSettings,
                                    decltype(mp::wrap(s))> // .
                                >;
                        })
                    .else_([](auto)
                        {
                            struct nope;
                            return nope{};
                        })(uws);
            });
    }

    template <typename TSettings, typename TSystemSignatureList>
    using system_storage_tuple_type =
        decltype(make_system_storage_tuple<TSettings>(TSystemSignatureList{}));
}
ECST_CONTEXT_STORAGE_SYSTEM_NAMESPACE_END
