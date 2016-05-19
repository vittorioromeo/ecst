// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/storage/component/chunk/type.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    template <typename TSettings>
    auto make_chunk_tuple(TSettings s) noexcept
    {
        auto csl = settings::component_signature_list(s);
        return bh::transform(csl,
            [](auto xe)
            {
                auto ct = signature::component::tag_of(xe);
                using c = signature::component::unwrap_tag<decltype(ct)>;

                return mp::type_c<chunk_dispatch_type<c, TSettings>>;
            }
            );
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
