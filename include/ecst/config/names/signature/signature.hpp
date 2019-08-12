// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config/names/global.hpp>

#define ECST_IMPL_SIGNATURE_NAMESPACE \
    ECST_NAMESPACE                    \
    {                                 \
        namespace signature

#define ECST_IMPL_SIGNATURE_NAMESPACE_END \
    }                                     \
    ECST_NAMESPACE_END

/// @macro Opens the `ecst::signature` namespace.
#define ECST_SIGNATURE_NAMESPACE ECST_IMPL_SIGNATURE_NAMESPACE

/// @macro Closes the `ecst::signature` namespace.
#define ECST_SIGNATURE_NAMESPACE_END ECST_IMPL_SIGNATURE_NAMESPACE_END
