// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./tag.hpp"

#define ECST_IMPL_TAG_COMPONENT_NAMESPACE \
    ECST_TAG_NAMESPACE                    \
    {                                     \
    namespace component

#define ECST_IMPL_TAG_COMPONENT_NAMESPACE_END \
    }                                         \
    ECST_TAG_NAMESPACE_END

/// @macro Opens the `ecst::tag::component` namespace.
#define ECST_TAG_COMPONENT_NAMESPACE ECST_IMPL_TAG_COMPONENT_NAMESPACE

/// @macro Closes the `ecst::tag::component` namespace.
#define ECST_TAG_COMPONENT_NAMESPACE_END ECST_IMPL_TAG_COMPONENT_NAMESPACE_END
