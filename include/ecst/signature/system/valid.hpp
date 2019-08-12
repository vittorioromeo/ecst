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
        using valid_impl = is_specialization_of_t<T, impl::data>;

        struct valid_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts...) const noexcept
            {
                return mp::list::all_variadic(impl::valid_impl<Ts>{}...);
            }
        };
    } // namespace impl

    /// @brief Evaluates to true if all `Ts...` are system signatures.
    constexpr impl::valid_t valid{};
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
