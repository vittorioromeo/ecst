// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"
#include <ecst/config.hpp>
#include <ecst/mp.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    template <typename... TComponentTags>
    constexpr auto make(TComponentTags... cts) noexcept;
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
