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
    template <typename TChunkTuple>
    auto make_chunk_metadata_tuple(TChunkTuple ct) noexcept
    {
        return mp::list::transform(
            [](auto xe)
            {
                using chunk_metadata =
                    chunk::metadata<mp::unwrap<decltype(xe)>>;

                return mp::type_v<chunk_metadata>;
            },
            ct);
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
