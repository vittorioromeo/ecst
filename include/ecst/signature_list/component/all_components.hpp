// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/component/signature.hpp>

namespace ecst::signature_list::component
{
    namespace impl
    {
        template <typename TComponentSignatureList>
        auto all_components_impl(TComponentSignatureList&& csl) noexcept
        {
            return bh::flatten(bh::transform(
                csl, [](auto cs) { return mp::unwrapped(cs).tag_list(); }));
        }
    } // namespace impl

    template <typename TComponentSignatureList>
    constexpr auto all_components(TComponentSignatureList csl) noexcept
    {
        return decltype(impl::all_components_impl(csl)){};
    }
} // namespace ecst::signature_list::component
