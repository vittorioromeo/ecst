// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/component/data.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        using valid_impl =
            mp::is_specialization_of<data, typename T::type>;
    }

    /// @brief Evaluates to true if all `Ts...` are component signatures.
    template <typename... Ts>
    constexpr auto valid(Ts...) noexcept
    {
        return mp::list::all_variadic(impl::valid_impl<Ts>{}...);
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
