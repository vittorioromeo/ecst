// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/signature/signature.hpp>

#define ECST_IMPL_SIGNATURE_SYSTEM_NAMESPACE \
    ECST_SIGNATURE_NAMESPACE                 \
    {                                        \
        namespace system

#define ECST_IMPL_SIGNATURE_SYSTEM_NAMESPACE_END \
    }                                            \
    ECST_SIGNATURE_NAMESPACE_END

/// @macro Opens the `ecst::signature::system` namespace.
#define ECST_SIGNATURE_SYSTEM_NAMESPACE ECST_IMPL_SIGNATURE_SYSTEM_NAMESPACE

/// @macro Opens the `ecst::signature::system` namespace.
#define ECST_SIGNATURE_SYSTEM_NAMESPACE_END \
    ECST_IMPL_SIGNATURE_SYSTEM_NAMESPACE_END
