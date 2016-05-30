// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/component/signature.hpp>

ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE
{
    template <typename TComponentSignatureList, typename TComponentSignature>
    constexpr auto id_by_signature(
        TComponentSignatureList csl, TComponentSignature cs) noexcept
    {
        ECST_S_ASSERT_DT(valid(csl));
        ECST_S_ASSERT_DT(signature::component::valid(mp::unwrapped(cs)));

        return mp::list::index_of(csl, cs);
    }


    template <typename TComponentSignatureList, typename TComponentTag>
    constexpr auto id_by_tag(
        TComponentSignatureList csl, TComponentTag ct) noexcept
    {
        return id_by_signature(csl, signature_by_tag(csl, ct));
    }
}
ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE_END
