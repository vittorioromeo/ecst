// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/component/tag.hpp>
#include <ecst/signature/component/tag_of.hpp>
#include <ecst/signature_list/component/is_signature_list.hpp>

ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE
{
    namespace impl
    {
        template <typename TComponentSignatureList, typename TComponentTag>
        constexpr auto signature_by_tag_impl(
            TComponentSignatureList csl, TComponentTag ct)
        {
            ECST_S_ASSERT_DT(is_signature_list(csl));
            ECST_S_ASSERT_DT(signature::component::is_tag(ct));

            return mp::list::find_first_matching(csl, [=](auto x_csig)
                {
                    return mp::same_type_decay(
                        signature::component::tag_of(x_csig), ct);
                });
        }
    }

    template <typename TComponentSignatureList, typename TComponentTag>
    constexpr auto signature_by_tag(
        TComponentSignatureList csl, TComponentTag ct)
    {
        return decltype(impl::signature_by_tag_impl(csl, ct)){};
    }

    template <typename TComponent, typename TComponentSignatureList>
    constexpr auto signature_by_type(TComponentSignatureList csl)
    {
        return signature_by_tag(csl, signature::component::tag<TComponent>);
    }
}
ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE_END
