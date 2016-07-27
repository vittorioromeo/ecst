// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include "./dependencies.hpp"

ECST_FUNCTION_QUEUE_NAMESPACE
{
    /// @brief Alias for a function pointer.
    template <typename TSignature>
    using fn_ptr = TSignature*;
}
ECST_FUNCTION_QUEUE_NAMESPACE_END
