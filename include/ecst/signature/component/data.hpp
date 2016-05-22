// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/tag/component.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        /// @brief Storage class for all the component signature data.
        template <typename TComponentTag>
        struct data
        {
            ECST_S_ASSERT_DT(tag::component::valid(TComponentTag{}));
            using tag = TComponentTag;
        };

        /// @brief Alias for the tag type inside `TComponentSignature`.
        template <typename TComponentSignature>
        using tag_type = typename mp::unwrap<TComponentSignature>::tag;
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
