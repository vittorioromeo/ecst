// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system/data.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        using is_signature_impl = mp::is_specialization_of<impl::data, T>;
    }

    /// @brief Evaluates to true if `T` is a system signature.
    template <typename T>
    constexpr auto is_signature(T)
    {
        return impl::is_signature_impl<T>{};
    }

    /// @brief Evaluates to true if all `Ts...` are system signatures.
    template <typename... Ts>
    constexpr auto are_signatures(Ts... xs)
    {
        return mp::list::all_variadic(is_signature(xs)...);
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
