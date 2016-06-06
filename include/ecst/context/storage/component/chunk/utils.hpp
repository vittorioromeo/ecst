// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/utils.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        /// @brief Chunk metadata type.
        /// @details Stored in a tuple, inside entity metadata.
        template <typename TChunk>
        using metadata = typename TChunk::metadata;
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
