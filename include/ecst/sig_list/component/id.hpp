// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/sig_list/component/sig.hpp>

namespace ecst::sig_list::component
{
    template <typename TComponentSignatureList, typename TComponentSignature>
    constexpr auto id_by_sig(
        TComponentSignatureList csl, TComponentSignature cs) noexcept
    {
        ECST_S_ASSERT_DT(valid(csl));
        ECST_S_ASSERT_DT(sig::component::valid(mp::unwrapped(cs)));

        return mp::list::index_of(csl, cs);
    }


    template <typename TComponentSignatureList, typename TComponentTag>
    constexpr auto id_by_tag(
        TComponentSignatureList csl, TComponentTag ct) noexcept
    {
        return id_by_sig(csl, sig_by_tag(csl, ct));
    }
} // namespace ecst::sig_list::component
