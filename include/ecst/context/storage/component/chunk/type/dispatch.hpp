// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "../utils.hpp"
#include "../impl.hpp"

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    // TODO:
    template <typename TSettings, typename TComponentSignature>
    using chunk_dispatch_type = ECST_DECAY_DECLTYPE(         // .
        TComponentSignature{}.make_storage_type(TSettings{}) // .
        );
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
