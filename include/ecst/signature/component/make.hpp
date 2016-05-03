// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/signature/component/data.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    /// @brief Creates a component signature from a tag.
    template <typename TComponentTag>
    constexpr auto make_by_tag(TComponentTag)
    {
        return impl::data<TComponentTag>{};
    }

    /// @brief Creates a component signature from a component type.
    template <typename TComponent>
    constexpr auto make()
    {
        return make_by_tag(tag<TComponent>);
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
