// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./data.hpp"

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    template <typename TSettings>
    using dispatch = impl::data<TSettings>;
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
