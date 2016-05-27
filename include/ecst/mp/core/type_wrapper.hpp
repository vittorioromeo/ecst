
// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>

ECST_MP_NAMESPACE
{
    // Bring hana's `type_c` into the `mp` namespace.
    using bh::type_c;

    /// @brief Returns the inner type wrapped by a `type_c`.
    template <typename T>
    using unwrap = typename T::type;

    namespace impl
    {
        // TODO: docs, test
        template <typename T>
        using is_wrapped = std::is_base_of<bh::basic_type<T>, T>;
    }

    /// @brief Returns a default-constructed instance of the inner type wrapped
    /// by a `type_c`.
    template <typename T>
    constexpr auto unwrapped(T) noexcept
    {
        return unwrap<T>{};
    }

    // TODO: docs
    template <typename T>
    constexpr auto wrap(T) noexcept
    {
        return bh::type_c<T>;
    }
}
ECST_MP_NAMESPACE_END
