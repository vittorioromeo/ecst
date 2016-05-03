// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include "./data.hpp"

ECST_SETTINGS_NAMESPACE
{
    template <                            // .
        typename TMultithreading,         // .
        typename TEntityStorage,          // .
        typename TSystemSignatureList,    // .
        typename TComponentSignatureList, // .
        typename TScheduler               // .
        >
    constexpr auto make(         // .
        TMultithreading,         // .
        TEntityStorage,          // .
        TSystemSignatureList,    // .
        TComponentSignatureList, // .
        TScheduler)
    {
        return impl::data<           // .
            TMultithreading,         // .
            TEntityStorage,          // .
            TSystemSignatureList,    // .
            TComponentSignatureList, // .
            TScheduler,              // .
            // TODO: allow customization:
            impl::refresh_parallelism::enabled // .
            >{};
    }
}
ECST_SETTINGS_NAMESPACE_END
