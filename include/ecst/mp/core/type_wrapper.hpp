
// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>

namespace ecst::mp
{
    // Bring hana's `type_c` into the `mp` namespace.
    using bh::type_c;

    /// @brief Returns the inner type wrapped by a `type_c`.
    template <typename T>
    using unwrap = typename T::type;

    /// @brief Returns a default-constructed instance of the inner type wrapped
    /// by a `type_c`.
    inline constexpr auto unwrapped = []<typename T>(T)
    {
        return unwrap<T>{};
    };

    /// @brief Returns a `type_c` wrapping the passed parameter.
    inline constexpr auto wrap = []<typename T>(T)
    {
        return type_c<T>;
    };
} // namespace ecst::mp
