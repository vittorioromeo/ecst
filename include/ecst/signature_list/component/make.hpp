// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/component.hpp>

ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE
{
    // TODO: rename
    namespace impl
    {
        template <typename... TComponentSignatures>
        constexpr auto make(TComponentSignatures... css)
        {
            ECST_S_ASSERT_DT(signature::component::are_signatures(css...));

            return mp::list::make(css...);
        }

        template <typename... TComponents>
        constexpr auto v = make(signature::component::make<TComponents>()...);
    }

    template <typename... TComponentTags>
    constexpr auto make(TComponentTags... cts)
    {
        // TODO: add storage type options
        ECST_S_ASSERT_DT(signature::component::are_tags(cts...));
        return impl::make(signature::component::make_by_tag(cts)...);
    }
}
ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE_END
