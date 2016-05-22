// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/tag/component.hpp>
#include <ecst/signature/component/valid.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    /// @brief Returns the tag type stored inside a component signature.
    template <typename TComponentSignature>
    constexpr auto tag_of(TComponentSignature cs) noexcept
    {
        ECST_S_ASSERT_DT(valid(cs));
        return impl::tag_type<TComponentSignature>{};
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
