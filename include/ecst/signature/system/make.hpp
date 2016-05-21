// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include "./data.hpp"
#include "./settings.hpp"

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    // TODO: builder pattern?

    template <                       // .
        typename TTag,               // .
        typename TParallelizability, // .
        typename TTagDependencyList, // .
        typename TComponentUseList,  // .
        typename TOutput             // .
        >
    constexpr auto make(    // .
        TTag,               // .
        TParallelizability, // .
        TTagDependencyList, // .
        TComponentUseList,  // .
        TOutput             // .
        )
    {
        return mp::bh::type_c<impl::data< // .
            TTag,                         // .
            TParallelizability,           // .
            TTagDependencyList,           // .
            TComponentUseList,            // .
            TOutput                       // .
            >>;
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
