// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/sig_list/system/id.hpp>

namespace ecst::sig_list::system
{
    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemTagList>
        auto sig_list_from_tag_list_impl(
            TSystemSignatureList ssl, TSystemTagList stl) noexcept
        {
            return bh::transform(
                stl, [ssl](auto st) { return sig_by_tag(ssl, st); });
        }
    } // namespace impl

    /// @brief Given a system signature list `ssl` and a list of system tags
    /// `stl`, returns a list of every tag in `stl` transformed to a signature.
    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto sig_list_from_tag_list(
        TSystemSignatureList ssl, TSystemTagList stl) noexcept
    {
        return decltype(impl::sig_list_from_tag_list_impl(ssl, stl)){};
    }
} // namespace ecst::sig_list::system
