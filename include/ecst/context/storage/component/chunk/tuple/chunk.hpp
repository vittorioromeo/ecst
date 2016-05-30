// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/context/storage/component/chunk/type.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    template <typename TSettings>
    auto make_chunk_tuple(TSettings s) noexcept
    {
        auto csl = settings::component_signature_list(s);
        return bh::transform(csl, [](auto cs)
            {
                using ct_type = mp::unwrap<ECST_DECAY_DECLTYPE(cs)>;
                return chunk_dispatch_type<TSettings, ct_type>{};
            });
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
