// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        template <typename TChunk>
        using metadata = typename TChunk::metadata;

        template <typename TChunk>
        using component = typename TChunk::component_type;
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
