// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./is_sig_list.hpp"
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/tag/component.hpp>

namespace ecst::sig_list::component
{
    namespace impl
    {
        template <typename TComponentSignatureList, typename TComponentTag>
        constexpr auto sig_by_tag_impl(
            TComponentSignatureList csl, TComponentTag ct)
        {
            ECST_S_ASSERT_DT(valid(csl));
            ECST_S_ASSERT_DT(tag::component::valid(ct));

            return *bh::find_if(
                csl, [ct](auto cs) { return mp::unwrapped(cs).has(ct); });
        }
    } // namespace impl

    template <typename TComponentSignatureList, typename TComponentTag>
    constexpr auto sig_by_tag(
        TComponentSignatureList csl, TComponentTag ct)
    {
        return decltype(impl::sig_by_tag_impl(csl, ct)){};
    }
} // namespace ecst::sig_list::component
