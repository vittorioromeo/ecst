// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        auto is_signature_list_impl()
        {
            return mp::list::all_match(T{}, [](auto sx)
                {
                    return signature::system::is_signature(sx);
                });
        }
    }

    /// @brief Returns `true` if the passed argument is a system signature list.
    template <typename T>
    constexpr auto is_signature_list(T)
    {
        return decltype(impl::is_signature_list_impl<T>()){};
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
