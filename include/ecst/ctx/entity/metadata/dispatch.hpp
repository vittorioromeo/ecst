// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./metadata.hpp"

namespace ecst::ctx::entity
{
    template <typename TSettings>
    using dispatch = impl::metadata<                                      // .
        ctx::bitset::dispatch<TSettings>,                             // .
        ctx::storage::component::chunk_metadata_tuple_type<TSettings> // .
        >;
}
