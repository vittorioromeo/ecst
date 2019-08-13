// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system.hpp>

namespace ecst::signature_list::system
{
    /// @brief Builds a system signature list from a variadic amount of system
    /// signatures.
    template <typename... TSystemSignatures>
    constexpr auto make(TSystemSignatures... sss)
    {
        ECST_S_ASSERT_DT(signature::system::valid(sss...));
        return mp::list::make(mp::wrap(sss)...);
    }
} // namespace ecst::signature_list::system
