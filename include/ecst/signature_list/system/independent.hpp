// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./id.hpp"
#include "./id_list.hpp"
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/system/signature_list_from_tag_list.hpp>

namespace ecst::signature_list::system
{
    template <typename TSystemSignatureList>
    constexpr auto independent(TSystemSignatureList ssl) noexcept
    {
        return bh::all_of(ssl, signature::system::indepedent);
    }

    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto independent_tag_list(
        TSystemSignatureList ssl, TSystemTagList stl) noexcept
    {
        return independent(signature_list_from_tag_list(ssl, stl));
    }
} // namespace ecst::signature_list::system
