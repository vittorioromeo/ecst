// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/system/signature.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    /// @brief Given a system signature `ss`, returns its ID.
    template <typename TSystemSignatureList, typename TSystemSignature>
    constexpr auto id_by_signature(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        // TODO:
        // ECST_S_ASSERT_DT(is_signature_list(ssl));
        // ECST_S_ASSERT_DT(signature::system::is_signature(ss));

        return mp::list::index_of(ssl, ss);
    }

    /// @brief Given a system type `TSystem`, returns its ID.
    template <typename TSystem, typename TSystemSignatureList>
    constexpr auto id_by_type(TSystemSignatureList ssl)
    {
        return id_by_signature(ssl, signature_by_type<TSystem>(ssl));
    }

    /// @brief Given a system tag `st`, returns its ID.
    template <typename TSystemSignatureList, typename TSystemTag>
    constexpr auto id_by_tag(TSystemSignatureList ssl, TSystemTag st)
    {
        ECST_S_ASSERT_DT(signature::system::is_tag(st));
        return id_by_signature(ssl, signature_by_tag(ssl, st));
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
