// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/global.hpp>

#define ECST_IMPL_TAG_NAMESPACE \
    ECST_NAMESPACE              \
    {                           \
        namespace tag

#define ECST_IMPL_TAG_NAMESPACE_END \
    }                               \
    ECST_NAMESPACE_END

/// @macro Opens the `ecst::tag` namespace.
#define ECST_TAG_NAMESPACE ECST_IMPL_TAG_NAMESPACE

/// @macro Closes the `ecst::tag` namespace.
#define ECST_TAG_NAMESPACE_END ECST_IMPL_TAG_NAMESPACE_END
