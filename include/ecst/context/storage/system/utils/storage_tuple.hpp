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
                using ssig_type = decltype(ssig);
                return mp::type_c<
                    context::system::instance<TSettings, ssig_type>>;
            });
    }

    template <typename TSettings, typename TSystemSignatureList>
    using system_storage_tuple_type =
        decltype(make_system_storage_tuple<TSettings>(TSystemSignatureList{}));
}
ECST_CONTEXT_STORAGE_SYSTEM_NAMESPACE_END
