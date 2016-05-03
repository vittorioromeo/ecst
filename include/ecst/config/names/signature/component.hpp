// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/signature/signature.hpp>

#define ECST_IMPL_SIGNATURE_COMPONENT_NAMESPACE \
    ECST_SIGNATURE_NAMESPACE                    \
    {                                           \
    namespace component

#define ECST_IMPL_SIGNATURE_COMPONENT_NAMESPACE_END \
    }                                               \
    ECST_SIGNATURE_NAMESPACE_END

/// @macro Opens the `ecst::signature::component` namespace.
#define ECST_SIGNATURE_COMPONENT_NAMESPACE \
    ECST_IMPL_SIGNATURE_COMPONENT_NAMESPACE

/// @macro Closes the `ecst::signature::component` namespace.
#define ECST_SIGNATURE_COMPONENT_NAMESPACE_END \
    ECST_IMPL_SIGNATURE_COMPONENT_NAMESPACE_END
