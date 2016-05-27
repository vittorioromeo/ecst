// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include "./id.hpp"
#include "./id_list.hpp"

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    template <typename TSystemSignatureList>
    constexpr auto independent(TSystemSignatureList ssl) noexcept
    {
        return mp::bh::all_of(ssl, signature::system::indepedent);
    }

    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto independent_tag_list(
        TSystemSignatureList ssl, TSystemTagList stl) noexcept
    {
        return independent(signature_list_from_tag_list(ssl, stl));
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
