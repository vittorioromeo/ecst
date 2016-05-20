// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system/data.hpp>
#include <ecst/signature/system/settings.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    template <                       // .
        typename TTag,               // .
        typename TParallelizability, // .
        typename TTagDependencyList, // .
        typename TComponentUseList,  // .
        typename TOutput             // .
        >
    constexpr auto make_by_tag( // .
        TTag,                   // .
        TParallelizability,     // .
        TTagDependencyList,     // .
        TComponentUseList,      // .
        TOutput                 // .
        )
    {
        return bh::type<impl::data< // .
            TTag,                   // .
            TParallelizability,     // .
            TTagDependencyList,     // .
            TComponentUseList,      // .
            TOutput                 // .
            >>{};
    }

    template <                       // .
        typename T,                  // .
        typename TParallelizability, // .
        typename TTagDependencyList, // .
        typename TComponentUseList,  // .
        typename TOutput             // .
        >
    constexpr auto make(    // .
        TParallelizability, // .
        TTagDependencyList, // .
        TComponentUseList,  // .
        TOutput             // .
        )
    {
        return make_by_tag(tag<T>, // .
            TParallelizability{},  // .
            TTagDependencyList{},  // .
            TComponentUseList{},   // .
            TOutput{}              // .
            );
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
