// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/context/storage/component/chunk/type.hpp>
#include <ecst/mp/list.hpp>

namespace ecst::context::storage::component
{
    template <typename TSettings>
    auto make_chunk_tuple(TSettings s) noexcept
    {
        auto csl = settings::component_signature_list(s);
        return bh::transform(csl,
            [s](auto cs) { return mp::unwrapped(cs).make_storage_type(s); });
    }
} // namespace ecst::context::storage::component
