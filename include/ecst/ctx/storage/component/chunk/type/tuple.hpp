// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../impl.hpp"
#include "../tuple.hpp"
#include "../utils.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::ctx::storage::component
{
    template <typename TSettings>
    using chunk_tuple_type =                  // .
        mp::list::unwrap_bh_tuple<            // .
            decltype(                         // .
                make_chunk_tuple(TSettings{}) // .
                )                             // .
            >;

    template <typename TSettings>
    using chunk_metadata_tuple_type =                                    // .
        mp::list::unwrap_bh_tuple<                                       // .
            decltype(                                                    // .
                make_chunk_metadata_tuple(make_chunk_tuple(TSettings{})) // .
                )                                                        // .
            >;
} // namespace ecst::ctx::storage::component
