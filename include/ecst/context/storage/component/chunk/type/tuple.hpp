// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/settings.hpp>
#include "../utils.hpp"
#include "../impl.hpp"
#include "../tuple.hpp"

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    template <typename TSettings>
    using chunk_tuple_type =                  // .
        mp::list::unwrap_bh_tuple<            // .
            decltype(                         // .
                make_chunk_tuple(TSettings{}) // .
                )                             // .
            >;

    // TODO: use bh tuple
    template <typename TSettings>
    using chunk_metadata_tuple_type =                                    // .
        mp::list::unwrap_tuple<                                          // .
            decltype(                                                    // .
                make_chunk_metadata_tuple(make_chunk_tuple(TSettings{})) // .
                )                                                        // .
            >;
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
